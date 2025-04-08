import serial
import time
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# 连接 Arduino 串口
ser = serial.Serial('COM6', 9600, timeout=1)  # 修改 COM 端口号
time.sleep(2)  # 等待串口稳定

# 存储数据
time_data = []
humidity_data = []
temperature_data = []
start_time = time.time()

# 初始化图像
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6), sharex=True)

def update(frame):
    global time_data, humidity_data, temperature_data

    try:
        line = ser.readline().decode('utf-8').strip()  # 读取一行数据
        if line.startswith("Time"):  # 忽略标题行
            return
        
        parts = line.split(',')
        if len(parts) >= 4:
            elapsed_time = float(parts[0])  # 时间
            humidity = float(parts[2])  # 湿度
            temperature = float(parts[3])  # 温度
            
            time_data.append(elapsed_time)
            humidity_data.append(humidity)
            temperature_data.append(temperature)

            # 限制数据长度（最多 300 秒）
            if elapsed_time > 300:
                time_data.pop(0)
                humidity_data.pop(0)
                temperature_data.pop(0)

            # 绘制湿度图
            ax1.clear()
            ax1.plot(time_data, humidity_data, label="Humidity (%)", color="blue", linewidth=2)
            ax1.set_title("Humidity Over Time")
            ax1.set_ylabel("Humidity (%)")
            ax1.legend(loc="upper right")
            ax1.grid(True)

            # 绘制温度图
            ax2.clear()
            ax2.plot(time_data, temperature_data, label="Temperature (°C)", color="red", linewidth=2)
            ax2.set_title("Temperature Over Time")
            ax2.set_xlabel("Time (s)")
            ax2.set_ylabel("Temperature (°C)")
            ax2.legend(loc="upper right")
            ax2.grid(True)

    except Exception as e:
        print("Error:", e)

# 实时更新曲线
ani = FuncAnimation(fig, update, interval=1000)
plt.tight_layout()
plt.show()
