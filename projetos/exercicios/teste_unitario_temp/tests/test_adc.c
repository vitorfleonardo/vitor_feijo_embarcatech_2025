// tests/test_adc.c
#include "unity.h"
#include "adc_temp.h"

void setUp(void) {}
void tearDown(void) {}

void test_adc_to_celsius_known_value() {
    // Simula uma leitura de ADC que gera exatamente 0.706V
    // 0.706V * 4095 / 3.3 ≈ 876
    uint16_t adc_simulado = 876;
    float temp = adc_to_celsius(adc_simulado);

    TEST_ASSERT_FLOAT_WITHIN(0.5, 27.0, temp); // Margem de erro de 0.5°C
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_adc_to_celsius_known_value);
    return UNITY_END();
}
