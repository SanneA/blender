#pragma once

#include "actions.hpp"
#include "force_interface.hpp"

namespace BParticles {

class Force {
 public:
  virtual ~Force() = 0;
  virtual void add_force(ForceInterface &interface) = 0;
};

class GravityForce : public Force {
 private:
  std::unique_ptr<ParticleFunction> m_compute_inputs;

 public:
  GravityForce(std::unique_ptr<ParticleFunction> compute_inputs)
      : m_compute_inputs(std::move(compute_inputs))
  {
  }

  void add_force(ForceInterface &interface) override;
};

class TurbulenceForce : public Force {
 private:
  std::unique_ptr<ParticleFunction> m_compute_inputs;

 public:
  TurbulenceForce(std::unique_ptr<ParticleFunction> compute_inputs)
      : m_compute_inputs(std::move(compute_inputs))
  {
  }

  void add_force(ForceInterface &interface) override;
};

}  // namespace BParticles
