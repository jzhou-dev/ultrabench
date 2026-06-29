#pragma once
#include <string>

class Runner {
public:
  Runner(std::string target) { _target = std::move(target); }
  virtual ~Runner() = default;

  virtual void execute() = 0;
  virtual std::string name() const = 0;

  const std::string &target() const { return _target; }

protected:
  std::string _target;
};
