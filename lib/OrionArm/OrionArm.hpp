#ifndef __ORION_ARMLIB__
#define __ORION_ARMLIB__

struct ArmCommand {
  enum class Command : int { Undefined = 0, Stop = 1, Fold = 1, Unfold = 2 };

  enum class Type : int { Undefined = 0, Move = 1, Command = 2 } type;
  int value;
};

struct ArmFeedback {};

template <std::size_t ID_LENGHT = 5, std::size_t COUNT = 5>
class OrionArm {
 public:
 private:
};

#endif