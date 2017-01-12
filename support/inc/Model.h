/**  Model - Header
*   @file       Model.h
*   @ingroup    GSAP-Support
*
*   @brief      Model class
*
*   @author     Matthew Daigle
*   @version    0.1.0
*
*   @pre        N/A
*
*      Contact: Matthew Daigle (matthew.j.daigle@nasa.gov)
*      Created: March 5, 2016
*
*   @copyright Copyright (c) 2016 United States Government as represented by
*     the Administrator of the National Aeronautics and Space Administration.
*     All Rights Reserved.
*/

#ifndef PCOE_MODEL_H
#define PCOE_MODEL_H

#include <vector>

namespace PCOE {
    // Trajectory typedef - state/input/output vector in time
    typedef std::vector<std::vector<double>> Trajectory;
    
    class Model {
    protected:
        unsigned int numStates;
        unsigned int numInputs;
        unsigned int numOutputs;
        double m_dt;  // Sampling time

    public:
        virtual ~Model() = default;

        /** @brief      Execute state equation. This version of the function uses the default sampling time.
        *   @param      t Time
        *   @param      x Current state vector. This gets updated to the state at the new time.
        *   @param      u Input vector
        *   @param      n Process noise vector
        **/
        void stateEqn(const double t, std::vector<double> & x, const std::vector<double> & u,
            const std::vector<double> & n);
        /** @brief      Execute state equation. This version of the function uses a given sampling time.
        *   @param      t Time
        *   @param      x Current state vector. This gets updated to the state at the new time.
        *   @param      u Input vector
        *   @param      n Process noise vector
        *   @param      dt Sampling time
        **/
        virtual void stateEqn(const double t, std::vector<double> & x,
            const std::vector<double> & u, const std::vector<double> & n,
            const double dt) = 0;
        /** @brief      Execute output equation
        *   @param      t Time
        *   @param      x State vector
        *   @param      u Input vector
        *   @param      n Sensor noise vector
        *   @param      z Output vector. This gets updated to the new output at the given time.
        **/
        virtual void outputEqn(const double t, const std::vector<double> & x,
            const std::vector<double> & u, const std::vector<double> & n,
            std::vector<double> & z) = 0;
        /** @brief      Initialize state vector given initial inputs and outputs.
        *   @param      x Current state vector. This gets updated.
        *   @param      u Input vector
        *   @param      z Output vector
        **/
        virtual void initialize(std::vector<double> & x, const std::vector<double> & u,
            const std::vector<double> & z) = 0;

        // Get size of vectors
        unsigned int getNumStates() const;
        unsigned int getNumInputs() const;
        unsigned int getNumOutputs() const;

        // Get/set dt
        double getDt() const;
        void setDt(const double newDt);
        
        // Simulate functions
        // 1. Assume sampling time and constant inputs
        void simulate(const double t0, const double tFinal, const std::vector<double> & x0, const std::vector<double> & u, Trajectory & X, Trajectory & Z);
        // 2. Assume sampling time, given input vector
        void simulate(const double t0, const double tFinal, const std::vector<double> & x0, const Trajectory & U, Trajectory & X, Trajectory & Z);
        // 3. Given time vector, assume constant inputs
        void simulate(const std::vector<double> & T, const std::vector<double> & x0, const std::vector<double> & u, Trajectory & X, Trajectory & Z);
        // 4. Given time vector and input trajectory
        void simulate(const std::vector<double> & T, const std::vector<double> & x0, const Trajectory & U, Trajectory & X, Trajectory & Z);
    };
}

#endif  // PCOE_MODEL_H
