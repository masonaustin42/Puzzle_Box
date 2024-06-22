import numpy as np
from scipy.optimize import minimize

def calibrate(data):
    def residuals(params, data):
        offset = params[:3]
        scale = params[3:6]
        soft_iron = np.reshape(params[6:], (3, 3))
        calibrated = (data - offset).dot(soft_iron) * scale
        error = np.linalg.norm(calibrated, axis=1) - 1
        return np.sum(error**2)

    # Initial guess: zero offsets, unity scales, identity matrix for soft iron
    initial_guess = np.zeros(12)
    initial_guess[3:6] = 1
    initial_guess[6:] = np.eye(3).flatten()

    result = minimize(residuals, initial_guess, args=(data,), method='L-BFGS-B')
    offset = result.x[:3]
    scale = result.x[3:6]
    soft_iron = np.reshape(result.x[6:], (3, 3))
    return offset, scale, soft_iron

# Load data from CSV file
data = np.loadtxt('output.csv', delimiter=',')

# Calibrate
offset, scale, soft_iron = calibrate(data)

print("Offsets:", offset)
print("Scales:", scale)
print("Soft Iron Matrix:", soft_iron)
