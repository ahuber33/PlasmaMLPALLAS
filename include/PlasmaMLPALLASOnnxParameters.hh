#ifndef PlasmaMLPALLASOnnxParameters_h
#define PlasmaMLPALLASOnnxParameters_h

/**
 * @class OnnxParameters
 * @brief Singleton class to manage ONNX beam parameters in a thread-safe way.
 * @author Arnaud HUBER <huber@lp2ib.in2p3.fr> - Alexei SYTOV <sytov@infn.it>
 * @date 2025
 * @copyright PALLAS Project - GEANT4 Collaboration
 *
 * This class provides thread-safe getters and setters for parameters used by 
 * ONNX models. It ensures that updates and reads are synchronized across multiple threads.
 */

#include <atomic>

class OnnxParameters {
public:
    /**
     * @brief Access the singleton instance.
     * @return Reference to the singleton instance of OnnxParameters.
     */
    static OnnxParameters& Instance() {
        static OnnxParameters instance;
        return instance;
    }

    /// @name Thread-safe setters
    ///@{
    void SetStatusONNX(int status) {
        std::lock_guard<std::mutex> lock(updateMutex);
        StatusONNX.store(status, std::memory_order_release);
    }

    void SetXoff(double x) {
        std::lock_guard<std::mutex> lock(updateMutex);
        Xoff.store(x, std::memory_order_release);
    }

    void SetA0(double a0) {
        std::lock_guard<std::mutex> lock(updateMutex);
        A0.store(a0, std::memory_order_release);
    }

    void SetCN2(double cn2) {
        std::lock_guard<std::mutex> lock(updateMutex);
        CN2.store(cn2, std::memory_order_release);
    }

    void SetPressure(double p) {
        std::lock_guard<std::mutex> lock(updateMutex);
        Pressure.store(p, std::memory_order_release);
    }

    void SetParticleName(const std::string& name) {
        std::lock_guard<std::mutex> lock(particleNameMutex);
        ParticleName = name;
    }
    ///@}

    /// @name Thread-safe getters
    ///@{
    int GetStatusONNX() const {
        return StatusONNX.load(std::memory_order_acquire);
    }

    double GetXoff() const {
        return Xoff.load(std::memory_order_acquire);
    }

    double GetA0() const {
        return A0.load(std::memory_order_acquire);
    }

    double GetCN2() const {
        return CN2.load(std::memory_order_acquire);
    }

    double GetPressure() const {
        return Pressure.load(std::memory_order_acquire);
    }

    std::string GetParticleName() const {
        std::lock_guard<std::mutex> lock(particleNameMutex);
        return ParticleName;
    }
    ///@}

private:
    std::atomic<int> StatusONNX{0};      /**< Atomic storage for ONNX status */
    std::atomic<double> Xoff{0.0};       /**< Atomic storage for X offset */
    std::atomic<double> A0{0.0};         /**< Atomic storage for beam amplitude */
    std::atomic<double> CN2{0.0};        /**< Atomic storage for CN2 */
    std::atomic<double> Pressure{0.0};   /**< Atomic storage for pressure */

    mutable std::mutex particleNameMutex; /**< Mutex to protect ParticleName */
    std::string ParticleName;             /**< Name of the particle */

    mutable std::mutex updateMutex;       /**< Mutex to protect full updates */

    /**
     * @brief Private constructor for singleton pattern.
     */
    OnnxParameters() = default;

    /**
     * @brief Destructor.
     */
    ~OnnxParameters() = default;

    OnnxParameters(const OnnxParameters&) = delete;            /**< Delete copy constructor */
    OnnxParameters& operator=(const OnnxParameters&) = delete; /**< Delete assignment operator */
};

#endif
