import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

#--------------------------------------------------------------------------------------------------
# Filters
def FIR(signal, N):
    x = [0] * N
    y = 0
    
    signal_processed = []
    
    for reading in signal:
        x.append(reading)
        x_out = x.pop(0)
        
        y += (reading - x_out) / N
        
        signal_processed.append(y)
        
    return np.array(signal_processed)

def IIR(signal, lam):
    y = 0
    
    signal_processed = []
    
    for reading in signal:
    
        y = lam * y + (1-lam) * reading
        
        signal_processed.append(y)
        
    return np.array(signal_processed)
    
def windowed_mean(signal, N):
    kernel = np.ones(N) / np.sum(np.ones(N))
    return np.correlate(signal, kernel, "same")

def edge_detection(signal):
    kernel = np.array([-1, 1])*5
    
    return np.correlate(signal, kernel, "same")

def edge_detection_large(signal, N, normalization):
    kernel = np.concatenate([
        -np.ones(N//2),
        np.ones(N//2),
    ]) * normalization
    
    return np.correlate(signal, kernel, "same")
    
#--------------------------------------------------------------------------------------------------
# Signal Setups
num_points = 500
t = np.linspace(0, 5 * np.pi, num_points) 

# sine wave
signal = np.sin(t)

# delta
# signal = np.zeros_like(t)
# signal[100] = 100

# square wave
signal = np.sign(np.sin(t))

signal_w_noise = signal + np.random.normal(0, 0.5, t.shape)

# signal_processed = FIR(signal_w_noise, 20)
# signal_processed = IIR(signal_w_noise, 0.99)
# signal_processed = windowed_mean(signal_w_noise, 20)
# signal_processed = edge_detection(signal_w_noise)

# signal_processed = edge_detection_large(signal_w_noise, 10, 0.1)

signal_processed = edge_detection(windowed_mean(signal, 20))

#--------------------------------------------------------------------------------------------------
# Plot Setups
fig, (ax1, ax2, ax3) = plt.subplots(nrows=3, sharex=True, figsize=(12, 12))

line_signal_w_noise, = ax1.plot([], [], 'r-', label="Noisy Signal")
line_signal, = ax1.plot([], [], 'k-', label="Signal")

line_signal_w_noise_p2, = ax2.plot([], [], 'r-', alpha=0.5, label="Noisy Signal")
line_signal_processed, = ax2.plot([], [], 'b-', label="Processed Signal")

line_signal_p3, = ax3.plot([], [], 'k-', label="Signal")
line_signal_processed_p3, = ax3.plot([], [], 'b-', label="Processed Signal")

ax1.set_xlim(0, 4 * np.pi)
ax1.set_ylim(-1.4, 1.4)
ax2.set_ylim(-1.4, 1.4)
ax3.set_ylim(-1.4, 1.4)

ax1.grid(True)
ax2.grid(True)
ax3.grid(True)

ax1.set_ylabel("Signal Amplitude")
ax2.set_ylabel("Signal Amplitude")
ax3.set_ylabel("Signal Amplitude")
ax3.set_xlabel("Time (t)")

ax1.legend()
ax2.legend()
ax3.legend()

# Initialize function
def init():
    print(line_signal)
    print(line_signal_w_noise)
    line_signal.set_data([], [])
    line_signal_w_noise.set_data([], [])
    
    line_signal_processed.set_data([], [])
    line_signal_w_noise_p2.set_data([], [])
    
    line_signal_p3.set_data([], [])
    line_signal_processed_p3.set_data([], [])
    
    return line_signal, line_signal_w_noise, line_signal_processed, line_signal_w_noise_p2, line_signal_processed_p3, line_signal_p3

# Update function for animation
def update(frame):
    line_signal.set_data(t[:frame], signal[:frame])
    line_signal_w_noise.set_data(t[:frame], signal_w_noise[:frame])
    
    line_signal_w_noise_p2.set_data(t[:frame], signal_w_noise[:frame])
    line_signal_processed.set_data(t[:frame], signal_processed[:frame])
    
    line_signal_p3.set_data(t[:frame], signal[:frame])
    line_signal_processed_p3.set_data(t[:frame], signal_processed[:frame])

    return line_signal, line_signal_w_noise, line_signal_processed, line_signal_w_noise_p2, line_signal_processed_p3, line_signal_p3

# Animate
ani = animation.FuncAnimation(fig, update, frames=num_points, init_func=init, blit=False, interval=10)

plt.show()
