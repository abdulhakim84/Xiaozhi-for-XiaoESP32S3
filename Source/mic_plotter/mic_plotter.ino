#include <driver/i2s.h>

#define I2S_WS 4   // LRCLK / WS
#define I2S_SCK 5  // BCLK
#define I2S_SD 6    // DATA from mic

#define SAMPLE_RATE 16000
#define BUFFER_SAMPLES 256

int32_t i2sBuffer[BUFFER_SAMPLES];

void setup() {
  Serial.begin(115200);
  delay(1000);

  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLE_RATE,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 4,
    .dma_buf_len = 256,
    .use_apll = false
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);

  Serial.println("Mic plotter started");
}

void loop() {
  size_t bytesRead = 0;

  i2s_read(
    I2S_NUM_0,
    (char*)i2sBuffer,
    sizeof(i2sBuffer),
    &bytesRead,
    portMAX_DELAY);

  int samples = bytesRead / sizeof(int32_t);

  for (int i = 0; i < samples; i++) {
    // Convert 32-bit I2S mic data to signed 16-bit
    int16_t sample = (int16_t)(i2sBuffer[i] >> 14);

    Serial.println(sample);
  }
}