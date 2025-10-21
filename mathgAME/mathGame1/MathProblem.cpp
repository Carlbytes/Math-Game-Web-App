//#include "MathProblem.h" // Link to the header file
//
//// Constructor definition
//MathProblem::MathProblem(int number1, int number2, char op, int answer);
//    : x(number1), y(number2), operation(op), answer(answer) {
//    // Constructor body is empty, initialization is done in the list
//}
//
//// --- Getter definitions ---
//int MathProblem::getX() const {
//    return x;
//}
//
//int MathProblem::getY() const {
//    return y;
//}
//
//char MathProblem::getOperation() const {
//    return operation;
//}
//
//int MathProblem::getAnswer() const {
//    return answer;
//}
//
//// --- Helper Method     definitions ---
//std::string MathProblem::getProblemString() const {
//    std::stringstream ss;
//    ss << x << " " << operation << " " << y << " = ?";
//    return ss.str();
//}
//
//std::string MathProblem::getSolutionString() const {
//    std::stringstream ss;
//    ss << x << " " << operation << " " << y << " = " << answer;
//    return ss.str();
//}