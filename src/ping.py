import time
import subprocess
import threading
import sys

def delayMilliseconds(milliseconds):
    time.sleep(milliseconds / 1000.0)

def process_packets(thread_local, time_delay):
    while getattr(thread_local, 'lag_enabled', False):
        # Process packets with a delay
        delayMilliseconds(time_delay)

def start_lag(time_delay):
    thread_local = threading.local()
    setattr(thread_local, 'lag_enabled', True)

    # Spoof lag by adding delay to outgoing ICMP ping packets
    command = [sys.executable, sys.argv[0], "--runas", "--start-lag", str(time_delay)]
    subprocess.run(command, shell=True)

def stop_lag():
    thread_local = threading.local()
    setattr(thread_local, 'lag_enabled', False)

    # Remove the lag spoofing configuration
    command = [sys.executable, sys.argv[0], "--runas", "--stop-lag"]
    subprocess.run(command, shell=True)

def main():
    print("Welcome to the Lag Spoofing Tool!")

    while True:
        command = input("Enter a command (start/stop/exit): ")

        if command == "start":
            time_delay = int(input("Enter the time delay (in milliseconds): "))
            start_lag(time_delay)
            print(f"Lag spoofing started with a delay of {time_delay} milliseconds.")
        elif command == "stop":
            stop_lag()
            print("Lag spoofing stopped.")
        elif command == "exit":
            break
        else:
            print("Invalid command. Please try again.")

if __name__ == "__main__":
    if "--runas" in sys.argv:
        if "--start-lag" in sys.argv:
            time_delay = int(sys.argv[sys.argv.index("--start-lag") + 1])
            process_packets(threading.local(), time_delay)
        elif "--stop-lag" in sys.argv:
            # Additional cleanup if required
            pass
    else:
        main()
