const char* ssid = "Man";
const char* password = "mansmnyu9";

WebServer server(80);

// ==============================
// OV2640 PINS
// ==============================

#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1

#define XCLK_GPIO_NUM    15
#define SIOD_GPIO_NUM     4
#define SIOC_GPIO_NUM     5

#define Y9_GPIO_NUM      16
#define Y8_GPIO_NUM      17
#define Y7_GPIO_NUM      18
#define Y6_GPIO_NUM      12
#define Y5_GPIO_NUM      10
#define Y4_GPIO_NUM       8
#define Y3_GPIO_NUM       9
#define Y2_GPIO_NUM      11

#define VSYNC_GPIO_NUM    6
#define HREF_GPIO_NUM     7
#define PCLK_GPIO_NUM    13


// ==============================
// CAMERA
// ==============================

bool startCamera()
{
    camera_config_t config = {};

    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;

    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;

    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;

    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;

    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;

    config.xclk_freq_hz = 10000000;

    config.pixel_format = PIXFORMAT_JPEG;

    // Keep this small because your board has NO PSRAM
    config.frame_size = FRAMESIZE_QQVGA;

    config.jpeg_quality = 20;

    config.fb_count = 1;

    config.fb_location = CAMERA_FB_IN_DRAM;

    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;

    Serial.println("Initializing camera...");

    esp_err_t result = esp_camera_init(&config);

    if (result != ESP_OK)
    {
        Serial.print("Camera failed: 0x");
        Serial.println(result, HEX);

        return false;
    }

    Serial.println("Camera initialized!");

    return true;
}


// ==============================
// WEB PAGE
// ==============================

void handleRoot()
{
    String page = R"rawliteral(
<!DOCTYPE html>
<html>

<head>

<meta name="viewport"
      content="width=device-width, initial-scale=1">

<title>ESP32 Live Camera</title>

<style>

body {
    margin: 0;
    background: #111;
    color: white;
    font-family: Arial;
    text-align: center;
}

h1 {
    margin: 20px;
}

.camera {
    width: 320px;
    max-width: 95%;
    border-radius: 10px;
}

.status {
    margin-top: 15px;
    color: #00ff88;
}

</style>

</head>

<body>

<h1>ESP32-S3 Live Camera</h1>

<img
    class="camera"
    src="/stream"
>

<div class="status">
    ● LIVE
</div>

</body>

</html>
)rawliteral";

    server.send(
        200,
        "text/html",
        page
    );
}


// ==============================
// MJPEG STREAM
// ==============================

void handleStream()
{
    WiFiClient client = server.client();

    client.print(
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n"
        "Cache-Control: no-cache\r\n"
        "Connection: close\r\n\r\n"
    );

    while (client.connected())
    {
        // Capture frame
        camera_fb_t* fb = esp_camera_fb_get();

        if (!fb)
        {
            Serial.println("Camera capture failed");
            break;
        }

        // Send boundary
        client.print("--frame\r\n");

        client.print(
            "Content-Type: image/jpeg\r\n"
        );

        client.print(
            "Content-Length: "
        );

        client.print(fb->len);

        client.print("\r\n\r\n");

        // Send image
        client.write(
            fb->buf,
            fb->len
        );

        client.print("\r\n");

        // Return framebuffer
        esp_camera_fb_return(fb);

        // Small delay
        delay(30);
    }

    client.stop();
}


// ==============================
// SETUP
// ==============================

void setup()
{
    Serial.begin(115200);

    delay(2000);

    Serial.println();
    Serial.println("==============================");
    Serial.println("ESP32-S3 LIVE CAMERA");
    Serial.println("==============================");

    // ==========================
    // CAMERA
    // ==========================

    if (!startCamera())
    {
        Serial.println("Camera startup failed!");

        while (true)
        {
            delay(1000);
        }
    }

    // ==========================
    // WIFI
    // ==========================

    Serial.println("Connecting to Wi-Fi...");

    WiFi.begin(
        ssid,
        password
    );

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);

        Serial.print(".");
    }

    Serial.println();

    Serial.println("Wi-Fi connected!");

    Serial.print("ESP32 IP address: ");

    Serial.println(
        WiFi.localIP()
    );

    // ==========================
    // WEB ROUTES
    // ==========================

    server.on(
        "/",
        HTTP_GET,
        handleRoot
    );

    server.on(
        "/stream",
        HTTP_GET,
        handleStream
    );

    server.begin();

    Serial.println("Web server started!");

    Serial.println();

    Serial.print("Open: http://");

    Serial.println(
        WiFi.localIP()
    );
}


// ==============================
// LOOP
// ==============================
