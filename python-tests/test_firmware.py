import serial
import time
import pytest

PICO_PORT = 'COM5'

@pytest.fixture(scope="module")
def pico_serial():
    print(f"\nConnecting to {PICO_PORT}...")
    try:
        ser = serial.Serial(PICO_PORT, baudrate=115200, timeout=1)
    except serial.SerialException as e:
        pytest.fail(f"Critical error: can't connect to port {PICO_PORT}. {e}")

    time.sleep(2)

    yield ser

    print(f"\nDisconnecting from {PICO_PORT}...")
    ser.close()

def test_connection(pico_serial):
    print("\n Start test connection")
    ser = pico_serial

    ser.write(b'PING\n')
    ser.readline()
    response = ser.readline().strip()
    print(f"Sent: PING, Recieved: {response}")

    assert response == b'PONG'
    print("Test done")

def test_led_logic(pico_serial):
    print("\nStart test led")
    ser = pico_serial

    ser.write(b'LED_STATUS\n')
    ser.readline()
    assert ser.readline().strip() == b'STATUS: OFF'

    ser.write(b'LED_ON\n')
    ser.readline()
    assert ser.readline().strip() == b'ACK'

    ser.write(b'LED_STATUS\n')
    ser.readline()
    assert ser.readline().strip() == b'STATUS: ON'

    ser.write(b'LED_OFF\n')
    ser.readline()
    assert ser.readline().strip() == b'ACK'

    ser.write(b'LED_STATUS\n')
    ser.readline()
    assert ser.readline().strip() == b'STATUS: OFF'

    print("Test done")

def test_smart_reading(pico_serial):
    print("\nStart test smart reading")
    ser = pico_serial

    ser.write(b'READ_SMART\n')
    ser.readline()
    response_bytes = ser.readline().strip()
    response_str = response_bytes.decode('utf-8')
    print(f"Recieved: {response_str}")

    assert response_str.startswith('SMART_TEMP:')

    temp_float = float(response_str.split(':')[1])
    assert 10.0 <= temp_float <= 50.0
    print(f"Temperature ok: {temp_float}°C")
    print("Test done")

def test_storage_logic(pico_serial):
    print("\nStart test storage")
    ser = pico_serial

    ser.write(b'WRITE 50 123\n')
    ser.readline()
    assert ser.readline().strip() == b'WRITE_OK 50'

    ser.write(b'WRITE 77 99\n')
    ser.readline()
    assert ser.readline().strip() == b'WRITE_OK 77'

    ser.write(b'READ 50\n')
    ser.readline()
    assert ser.readline().strip() == b'READ_OK 50 123'

    ser.write(b'READ 77\n')
    ser.readline()
    assert ser.readline().strip() == b'READ_OK 77 99'

    print("Test done")