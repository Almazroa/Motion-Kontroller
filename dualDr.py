import argparse
import asyncio
import contextlib
import logging
from typing import Iterable
import pyautogui
import time
from bleak import BleakClient, BleakScanner

BLE_R_IMU_left        = 0
BLE_R_IMU_right         = 1
BLE_R_IMU_up          = 2
BLE_R_IMU_down        = 3
BLE_R_bumper         = 4
BLE_R_trigger          = 5
BLE_R_button_up       = 6
BLE_R_button_down     = 7
BLE_R_button_right    = 8
BLE_R_button_left     = 9
BLE_L_IMU_left        = 10
BLE_L_IMU_right       = 11
BLE_L_IMU_up          = 12
BLE_L_IMU_down        = 13
BLE_L_trigger         = 14
BLE_L_bumper          = 15
BLE_L_button_up       = 16
BLE_L_button_down     = 17
BLE_L_button_right    = 18
BLE_L_button_left     = 19
BLE_ERROR             = 20                

last_key = '0'
focused = False

def start_game():
    pyautogui.moveTo(960,540)
    pyautogui.click()
def mouse_left():
    pyautogui.move(-100, 0, duration=0.25)
def mouse_right():
    pyautogui.move(100, 0, duration=0.25)
def mouse_up():
    pyautogui.move(0, -100, duration=0.25)
def mouse_down():
    pyautogui.move(0, 100, duration=0.25)
def mouse_click(side):
        pyautogui.mouseDown(button=side)
        time.sleep(0.1)
        pyautogui.mouseUp(button=side)

async def connect_to_device(
    lock: asyncio.Lock,
    by_address: bool,
    macos_use_bdaddr: bool,
    name_or_address: str,
    notify_uuid: str,
):
    """
    Scan and connect to a device then print notifications for 10 seconds before
    disconnecting.

    Args:
        lock:
            The same lock must be passed to all calls to this function.
        by_address:
            If true, treat *name_or_address* as an address, otherwise treat
            it as a name.
        macos_use_bdaddr:
            If true, enable hack to allow use of Bluetooth address instead of
            UUID on macOS.
        name_or_address:
            The Bluetooth address/UUID of the device to connect to.
        notify_uuid:
            The UUID of a characteristic that supports notifications.
    """
    logging.info("starting %s task", name_or_address)

    try:
        async with contextlib.AsyncExitStack() as stack:

            # Trying to establish a connection to two devices at the same time
            # can cause errors, so use a lock to avoid this.
            async with lock:
                logging.info("scanning for %s", name_or_address)

                if by_address:
                    device = await BleakScanner.find_device_by_address(
                        name_or_address, macos=dict(use_bdaddr=macos_use_bdaddr)
                    )
                else:
                    device = await BleakScanner.find_device_by_name(name_or_address)

                logging.info("stopped scanning for %s", name_or_address)

                if device is None:
                    logging.error("%s not found", name_or_address)
                    return

                client = BleakClient(device)

                logging.info("connecting to %s", name_or_address)

                await stack.enter_async_context(client)

                logging.info("connected to %s", name_or_address)

                # This will be called immediately before client.__aexit__ when
                # the stack context manager exits.
                stack.callback(logging.info, "disconnecting from %s", name_or_address)

            # The lock is released here. The device is still connected and the
            # Bluetooth adapter is now free to scan and connect another device
            # without disconnecting this one.

            def callback(_, data):
            #def notification_handler(sender, data):
                global last_key
                global focused
                keyPress = False
                print(f"Received notification: {data}")
                data_int = (int.from_bytes(data, byteorder='little', signed=False))
                if data_int == BLE_R_IMU_left: # mouse left
                    mouse_left()
                elif data_int == BLE_R_IMU_right: # mouse right
                    mouse_right()
                elif data_int == BLE_R_IMU_up: # mouse up
                    mouse_up()
                elif data_int == BLE_R_IMU_down: # mouse down
                    mouse_down()
                elif data_int == BLE_L_bumper:  
                    pass
                elif data_int == BLE_L_trigger: # left mouse click
                    mouse_click('left')
                elif data_int == BLE_R_bumper:  
                    pass
                elif data_int == BLE_R_trigger: # right mouse click
                    if (focused):
                        pyautogui.mouseUp(button='right')
                    else:
                        pyautogui.mouseDown(button='right')
                    #mouse_click('right')
                    focused = not focused
                elif data_int == BLE_R_button_up:
                    start_game()
                else: # handle all key press functions
                    keyPress = True
                    if data_int == BLE_R_button_down: # G key
                        curr_key = 'g'
                    elif data_int == BLE_R_button_right: # R key
                        curr_key = 'r'
                    elif data_int == BLE_R_button_left: # Q key
                        curr_key = 'q'
                    elif data_int == BLE_L_IMU_left: # A key
                        curr_key = 'a'
                    elif data_int == BLE_L_IMU_right: # D key
                        curr_key = 'd'
                    elif data_int == BLE_L_IMU_up: # W key
                        curr_key = 'w'
                    elif data_int == BLE_L_IMU_down: # S key
                        curr_key = 's'
                    elif data_int == BLE_L_button_up: # esc key
                        curr_key = 'esc'
                    elif data_int == BLE_L_button_down: # tab key
                        curr_key = 'tab'
                    elif data_int == BLE_L_button_right: # shift key
                        curr_key = 'shift'
                    elif data_int == BLE_L_button_left: # E key
                        curr_key = 'e'
                    else: # not a key press
                        key_press = False
                if(keyPress):
                    if last_key != curr_key:
                        pyautogui.keyUp(last_key)
                        last_key = curr_key
                        pyautogui.keyDown(curr_key)
                    else:
                        last_key = curr_key
                else:
                    pyautogui.keyUp(last_key)
                            

            #await client.pair()
            logging.info("Paired to %s", name_or_address)
            await client.start_notify(notify_uuid, callback,force_indicate=True)
            while client.is_connected():
                await asyncio.sleep(5)
            #await client.stop_notify(notify_uuid)

        # The stack context manager exits here, triggering disconnection.

        logging.info("disconnected from %s", name_or_address)

    except Exception:
        logging.exception("error with %s", name_or_address)


async def main(
    by_address: bool,
    macos_use_bdaddr: bool,
    addresses: Iterable[str],
    uuids: Iterable[str],
):
    lock = asyncio.Lock()
    while True:
        await asyncio.gather(
            *(
                connect_to_device(lock, by_address, macos_use_bdaddr, address, uuid)
                for address, uuid in zip(addresses, uuids)
            )
        )
        await asyncio.sleep(10)
    


if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "device1",
        metavar="<device>",
        help="Bluetooth name or address of first device connect to",
    )
    parser.add_argument(
        "uuid1",
        metavar="<uuid>",
        help="notification characteristic UUID on first device",
    )
    parser.add_argument(
        "device2",
        metavar="<device>",
        help="Bluetooth name or address of second device to connect to",
    )
    parser.add_argument(
        "uuid2",
        metavar="<uuid>",
        help="notification characteristic UUID on second device",
    )

    parser.add_argument(
        "--by-address",
        action="store_true",
        help="when true treat <device> args as Bluetooth address instead of name",
    )

    parser.add_argument(
        "--macos-use-bdaddr",
        action="store_true",
        help="when true use Bluetooth address instead of UUID on macOS",
    )

    parser.add_argument(
        "-d",
        "--debug",
        action="store_true",
        help="sets the log level to debug",
    )

    args = parser.parse_args()

    log_level = logging.DEBUG if args.debug else logging.INFO
    logging.basicConfig(
        level=log_level,
        format="%(asctime)-15s %(name)-8s %(levelname)s: %(message)s",
    )

    asyncio.run(
        main(
            args.by_address,
            args.macos_use_bdaddr,
            (args.device1, args.device2),
            (args.uuid1, args.uuid2),
        )
    )