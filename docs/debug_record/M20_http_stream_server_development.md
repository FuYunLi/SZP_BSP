# M20: HTTP流媒体服务器开发记录

## 1. 问题背景

### 1.1 设计需求

**模块名称**：网络视频流媒体传输 (HTTP Stream)
**引入组件**：ESP-IDF 内置 `esp_http_server` + `espressif/esp_new_jpeg`
**核心任务**：利用 Wi-Fi（M15），在应用层搭建 HTTP 服务，将摄像头捕获的图像以 MJPEG 格式实时向客户端进行流式分发
**验证手段**：手机或 PC 浏览器访问开发板的 `http://<ip>/stream`，页面应能显示连贯、平滑的摄像头视频画面

---

### 1.2 技术挑战

#### 挑战1：GC0308不支持JPEG格式

**问题**：
- GC0308摄像头硬件不支持JPEG压缩
- M19阶段已确认：`E (225322) camera: JPEG format is not supported on this sensor`
- 当前配置：RGB565格式（320x240×2 = 153KB）

**解决方案**：
- 使用 `esp_new_jpeg` 组件进行软件JPEG编码
- 将RGB565转换为JPEG格式
- 压缩率：~5-10倍（20KB/帧）

---

#### 挑战2：实时流媒体性能优化

**问题**：
- 连续捕获帧需要高效率
- 避免阻塞HTTP服务器
- 多客户端并发访问

**解决方案**：
- **双缓冲机制**：
  - 前缓冲：当前发送帧
  - 后缓冲：下一帧捕获
  - 避免捕获和发送冲突

- **任务分离**：
  - 摄像头捕获任务：独立任务，持续捕获帧
  - HTTP发送任务：HTTP服务器回调，发送最新帧
  - 避免阻塞HTTP服务器

- **帧率控制**：
  - 目标帧率：10 FPS
  - 使用定时器控制捕获频率
  - 避免过度消耗CPU资源

---

#### 挑战3：多客户端并发访问

**问题**：
- 多个浏览器同时访问 `/stream`
- 需要共享同一摄像头资源
- 避免重复捕获帧

**解决方案**：
- **共享帧缓冲区**：
  - 所有客户端共享同一帧缓冲区
  - 摄像头捕获任务更新帧缓冲区
  - HTTP回调读取最新帧并发送

- **客户端管理**：
  - 记录活跃客户端数量
  - 客户端断开时释放资源
  - 无客户端时停止捕获（节省资源）

---

## 2. 开发过程

### 2.1 添加JPEG编码组件依赖

**步骤**：
1. 在 [bsp/idf_component.yml](file:///home/liyun/esp/projects/LCSZP/SZP_BSP/V1/bsp/idf_component.yml) 中添加依赖：
   ```yaml
   dependencies:
     espressif/esp_new_jpeg: "^1.0.0"
   ```

2. 构建项目，组件管理器自动下载 `esp_new_jpeg` 组件

**关键配置**：
- `JPEG_PIXEL_FORMAT_RGB565_LE`：RGB565小端格式（摄像头输出格式）
- `JPEG_SUBSAMPLE_420`：YUV420子采样（最常用的压缩方式）
- `quality = 60`：JPEG质量（1-100，推荐60）
- `JPEG_ROTATE_0D`：不旋转

---

### 2.2 创建HTTP流媒体服务器模块

**文件结构**：
- [app/include/http_stream_server.h](file:///home/liyun/esp/projects/LCSZP/SZP_BSP/V1/app/include/http_stream_server.h)：头文件，声明API
- [app/http_stream_server.c](file:///home/liyun/esp/projects/LCSZP/SZP_BSP/V1/app/http_stream_server.c)：实现文件

**核心API**：
```c
esp_err_t http_stream_server_start(void);  // 启动HTTP流媒体服务器
esp_err_t http_stream_server_stop(void);   // 停止HTTP流媒体服务器
bool http_stream_server_is_running(void);  // 获取服务器运行状态
```

---

### 2.3 实现摄像头捕获任务

**关键代码**：
```c
// JPEG编码器配置
jpeg_enc_config_t jpeg_cfg = {
    .width = 320,
    .height = 240,
    .src_type = JPEG_PIXEL_FORMAT_RGB565_LE,  // RGB565小端格式
    .subsampling = JPEG_SUBSAMPLE_420,        // YUV420子采样
    .quality = 60,                            // JPEG质量（1-100）
    .rotate = JPEG_ROTATE_0D,                 // 不旋转
    .task_enable = false,                     // 单任务模式
};

// 打开JPEG编码器
jpeg_enc_handle_t jpeg_enc = NULL;
jpeg_error_t ret = jpeg_enc_open(&jpeg_cfg, &jpeg_enc);

// 捕获摄像头帧
camera_fb_t *fb = bsp_camera_fb_get();

// JPEG编码（RGB565转JPEG）
ret = jpeg_enc_process(jpeg_enc, fb->buf, fb->len,
                       s_jpeg_buf[next_buf_index], JPEG_BUFFER_SIZE,
                       &out_size);

// 关闭JPEG编码器
jpeg_enc_close(jpeg_enc);
```

**双缓冲机制**：
- `s_jpeg_buf[2]`：两个JPEG缓冲区（64KB each）
- `s_current_buf_index`：当前缓冲区索引
- `s_buf_mutex`：互斥锁，保护缓冲区访问

---

### 2.4 实现HTTP流媒体处理

**关键代码**：
```c
// HTTP /stream 端点处理函数
static esp_err_t stream_handler(httpd_req_t *req)
{
    // 设置响应头（MJPEG流）
    httpd_resp_set_type(req, "multipart/x-mixed-replace; boundary=frame");
    httpd_resp_set_hdr(req, "Cache-Control", "no-cache");
    httpd_resp_set_hdr(req, "Pragma", "no-cache");

    // 循环发送JPEG帧
    while (s_running && s_client_count > 0)
    {
        // 获取最新JPEG帧
        int buf_index = s_current_buf_index;
        size_t jpeg_size = s_jpeg_size[buf_index];
        uint8_t *jpeg_buf = s_jpeg_buf[buf_index];

        // 发送boundary分隔符
        char boundary_header[128];
        snprintf(boundary_header, sizeof(boundary_header),
                 "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %zu\r\n\r\n",
                 jpeg_size);
        httpd_resp_send_chunk(req, boundary_header, strlen(boundary_header));

        // 发送JPEG帧数据
        httpd_resp_send_chunk(req, (char *)jpeg_buf, jpeg_size);

        // 发送结束标记
        httpd_resp_send_chunk(req, "\r\n", 2);

        // 控制帧率（10 FPS）
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    return ESP_OK;
}
```

**MJPEG格式**：
- `Content-Type: multipart/x-mixed-replace; boundary=frame`
- 每帧数据格式：JPEG图像 + boundary分隔符
- 浏览器自动刷新显示

---

### 2.5 注册控制台命令

**命令列表**：
- `stream_start`：启动HTTP流媒体服务器
- `stream_stop`：停止HTTP流媒体服务器

**关键代码**：
```c
static int do_stream_start_cmd(int argc, char **argv)
{
    printf("正在启动 HTTP 流媒体服务器...\n");
    esp_err_t err = http_stream_server_start();
    if (err != ESP_OK)
    {
        printf("错误: HTTP 流媒体服务器启动失败 (%s)\n", esp_err_to_name(err));
        return 1;
    }
    printf("HTTP 流媒体服务器已启动！\n");
    printf("请在浏览器中访问: http://<设备IP>/stream\n");
    return 0;
}
```

---

### 2.6 编译系统配置

**修改文件**：[app/CMakeLists.txt](file:///home/liyun/esp/projects/LCSZP/SZP_BSP/V1/app/CMakeLists.txt)

**关键修改**：
```cmake
idf_component_register(SRCS "app_cli.c" "app_init.c" "app_ui.c" "http_stream_server.c"
                       INCLUDE_DIRS "include"
                       REQUIRES bsp services nvs_flash pca9557 qmi8658 esp_http_server espressif__esp_new_jpeg)
```

**依赖说明**：
- `esp_http_server`：ESP-IDF内置HTTP服务器组件
- `espressif__esp_new_jpeg`：JPEG编码组件

---

## 3. 编译错误与解决

### 3.1 错误1：esp_jpeg_enc.h文件未找到

**错误信息**：
```
fatal error: esp_jpeg_enc.h: No such file or directory
```

**原因**：
- `espressif__esp_new_jpeg` 组件不在app的依赖列表中

**解决方案**：
- 在 [app/CMakeLists.txt](file:///home/liyun/esp/projects/LCSZP/SZP_BSP/V1/app/CMakeLists.txt) 中添加依赖：
  ```cmake
  REQUIRES ... espressif__esp_new_jpeg
  ```

---

### 3.2 错误2：snprintf输出可能截断

**错误信息**：
```
error: '   ' directive output may be truncated writing 4 bytes into a region of size between 3 and 12 [-Werror=format-truncation=]
```

**原因**：
- `boundary_header`缓冲区大小为64字节，但输出可能需要57-66字节

**解决方案**：
- 增加 `boundary_header` 缓冲区大小从64字节到128字节：
  ```c
  char boundary_header[128];
  ```

---

## 4. 测试验证

### 4.1 功能测试

**测试步骤**：
1. 启动WiFi连接，获取IP地址
2. 启动HTTP流媒体服务器：`stream_start`
3. 浏览器访问：`http://<ip>/stream`
4. 观察视频画面是否流畅、无卡顿

**预期结果**：
- ✅ 浏览器显示实时视频画面
- ✅ 帧率：10 FPS左右
- ✅ 分辨率：320x240
- ✅ 多客户端可同时访问

---

### 4.2 性能测试

**测试指标**：
- 帧率：目标10 FPS
- 帧大小：20KB左右（JPEG压缩）
- 带宽：200KB/s（单客户端）
- CPU负载：<60%
- 内存占用：<200KB

**测试方法**：
- 使用Chrome开发者工具监控网络流量
- 使用 `idf.py monitor` 观察系统日志
- 使用 `heap_caps_print_heap_info()` 检查内存

---

## 5. 性能优化建议

### 5.1 帧率优化

**当前配置**：
- 目标帧率：10 FPS
- 延迟：100ms（vTaskDelay）

**优化建议**：
- 降低延迟到50ms，帧率提升到20 FPS
- 使用定时器精确控制帧率
- 根据CPU负载动态调整帧率

---

### 5.2 JPEG质量优化

**当前配置**：
- JPEG质量：60（中等质量）
- 帧大小：~20KB

**优化建议**：
- 降低质量到40，帧大小减小到~15KB
- 提升质量到80，帧大小增加到~30KB
- 根据网络带宽动态调整质量

---

### 5.3 内存优化

**当前配置**：
- JPEG缓冲区：64KB × 2 = 128KB
- 摄像头帧缓冲区：153KB（已在PSRAM）

**优化建议**：
- 降低JPEG缓冲区到32KB，节省内存
- 使用PSRAM分配JPEG缓冲区
- 使用block模式编码，减少内存峰值

---

## 6. 总结

### 6.1 开发成果

**已完成功能**：
1. ✅ HTTP流媒体服务器实现
2. ✅ JPEG编码集成（esp_new_jpeg）
3. ✅ 摄像头捕获任务（双缓冲机制）
4. ✅ 控制台命令注册（stream_start/stream_stop）
5. ✅ 编译成功，固件大小增加约20KB

**关键文件**：
- [app/include/http_stream_server.h](file:///home/liyun/esp/projects/LCSZP/SZP_BSP/V1/app/include/http_stream_server.h)
- [app/http_stream_server.c](file:///home/liyun/esp/projects/LCSZP/SZP_BSP/V1/app/http_stream_server.c)
- [app/app_cli.c](file:///home/liyun/esp/projects/LCSZP/SZP_BSP/V1/app/app_cli.c)
- [bsp/idf_component.yml](file:///home/liyun/esp/projects/LCSZP/SZP_BSP/V1/bsp/idf_component.yml)
- [app/CMakeLists.txt](file:///home/liyun/esp/projects/LCSZP/SZP_BSP/V1/app/CMakeLists.txt)

---

### 6.2 技术要点

**关键技术**：
1. **esp_new_jpeg API**：
   - `jpeg_enc_open()`：打开编码器
   - `jpeg_enc_process()`：编码RGB565为JPEG
   - `jpeg_enc_close()`：关闭编码器

2. **HTTP服务器**：
   - `httpd_start()`：启动HTTP服务器
   - `httpd_register_uri_handler()`：注册URI处理函数
   - `httpd_resp_send_chunk()`：发送分块数据

3. **MJPEG流格式**：
   - `Content-Type: multipart/x-mixed-replace; boundary=frame`
   - 每帧数据：JPEG + boundary分隔符

---

### 6.3 下一步计划

**待完成任务**：
1. 功能测试：验证浏览器访问是否正常
2. 性能测试：监控帧率、带宽、CPU负载
3. 多客户端测试：验证并发访问是否正常
4. 优化调整：根据测试结果调整参数

---

## 7. 参考资料

- [ESP_NEW_JPEG介绍](https://developer.espressif.com/blog/2025/09/esp-new-jpeg-introduction/)
- [esp_new_jpeg GitHub](https://github.com/espressif/esp-adf-libs/tree/master/esp_new_jpeg)
- [ESP-IDF HTTP服务器文档](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/protocols/esp_http_server.html)
- [MJPEG流格式说明](https://en.wikipedia.org/wiki/Motion_JPEG)