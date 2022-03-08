
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>  // for allocator, __shared_ptr_access, shared_ptr
#include <string>  // for wstring, basic_string
#include <vector>  // for vector
/**
 * @brief calls acpi interface
 *
 * @param request string of acpi request
 *
 * @return
 */
std::string callACPI(const std::string& request) {
  FILE* f = fopen("/proc/acpi/call", "w+t");
  if (!f) {
    perror("unable to open /proc/acpi/call");
    exit(2);
  }

  fprintf(f, "%s\n", request.c_str());
  char buf[200];
  size_t r = fread(buf, 1, sizeof(buf), f);
  fclose(f);

  return {buf, buf + r};
}
/**
 * @brief executes the acpi status and gives th output
 *
 * @param acpiCall command to be executed
 *
 * @return output of the command
 */
int acpiStatus(const std::string& acpiCall) {
  system(("echo '" + acpiCall + "' | sudo tee /proc/acpi/call >/dev/null")
             .c_str());
  std::ifstream ifs("/proc/acpi/call");
  std::string ret{std::istreambuf_iterator<char>(ifs),
                  std::istreambuf_iterator<char>()};
  ifs.close();  // must close the inout stream so the file can be cleaned up
  std::string pos = ret.substr(2, 1);
  return std::stoi(pos);
}
int fanStatus() {
  int result = acpiStatus("\\_SB.PCI0.LPC0.EC0.SPMO");
  return result;
}
int rapidChargeStatus(){
  int result = acpiStatus("\\_SB.PCI0.LPC0.EC0.QCHO");
  return result;
}
int consvervationModeStatus(){
  int result = acpiStatus("\\_SB.PCI0.LPC0.EC0.BTSM");
  return result;
}
//TODO add functions for toggling acpi modules 
void toggleRapidCharge(int state){
        if(state == 0){
                callACPI("\\_SB.PCI0.LPC0.EC0.VPC0.SBMC 0x08");
        }
        else{
                callACPI("\\_SB.PCI0.LPC0.EC0.VPC0.SBMC 0x07");
        }
}
void toggleConservationMode(int state){

        if(state == 1){
                callACPI("\\_SB.PCI0.LPC0.EC0.VPC0.SBMC 0x03");
                toggleRapidCharge(0);
        }
        else{
                callACPI("\\_SB.PCI0.LPC0.EC0.VPC0.SBMC 0x05");
        }
}
