#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>  // for allocator, __shared_ptr_access, shared_ptr
#include <string>  // for wstring, basic_string
#include <vector>  // for vector

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Radiobox, Horizontal, Menu, Renderer, Tab
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for Element, separator, hbox, operator|, border

using namespace ftxui;
char conservationStr[80];
char fnLockStr[80];
struct RadioboxOption tab1;
struct RadioboxOption tab2;
struct RadioboxOption tab3;

/**
 * @brief executes the system command and gives th output
 *
 * @param command command to be executed
 *
 * @return output of the command
 */
int execute(const std::string& command) {
  system((command + " > temp.txt").c_str());

  std::ifstream ifs("temp.txt");
  std::string ret{std::istreambuf_iterator<char>(ifs),
                  std::istreambuf_iterator<char>()};
  ifs.close();  // must close the inout stream so the file can be cleaned up
  if (std::remove("temp.txt") != 0) {
    perror("Error deleting temporary file");
  }
  return std::stoi(ret);
}

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
int main(int argc, const char* argv[]) {
  int value = 50;

  int tab_selected = 0;
  int tab_1_selected = execute(
      "cat "
      "/sys/bus/platform/drivers/ideapad_acpi/VPC2004:00/conservation_mode");
  int tab_2_selected =
      execute("cat /sys/bus/platform/drivers/ideapad_acpi/VPC2004:00/fn_lock");
  int tab_3_selected = 0;

  std::vector<std::wstring> tab_values{
      L"Conservation Mode",
      L"fn_lock",
      L"Battery saving",
      L"Rapid charging",
  };
  auto tab_menu = Menu(&tab_values, &tab_selected);

  std::vector<std::wstring> tab_1_entries{
      L"Off",
      L"On",
  };
  std::vector<std::wstring> tab_2_entries{
      L"Off",
      L"On",
  };
  std::vector<std::wstring> tab_3_entries{
      L"intelligent_cooling",
      L"extreme_performance",
      L"battery_saving",
  };
  std::vector<std::wstring> tab_4_entries{
      L"Off",
      L"On",
  };
  tab1.on_change = [&] {
    if (tab_1_selected) {
      callACPI("\\_SB.PCI0.LPC0.EC0.VPC0.SBMC 0x03");
    } else {
      callACPI("\\_SB.PCI0.LPC0.EC0.VPC0.SBMC 0x05");
    }
  };
  tab2.on_change = [&] {
    sprintf(fnLockStr,
            "echo %d > "
            "/sys/bus/platform/drivers/ideapad_acpi/"
            "VPC2004:00/fn_lock",
            tab_2_selected);
    system(fnLockStr);
  };
  tab3.on_change = [&] {
    if (tab_3_selected == 0) {
      callACPI("\\_SB.PCI0.LPC0.EC0.VPC0.DYTC 0x000FB001");
    } else if (tab_3_selected == 1) {
      callACPI("\\_SB.PCI0.LPC0.EC0.VPC0.DYTC 0x0012B001");
    } else if (tab_3_selected == 2) {
      callACPI("\\_SB.PCI0.LPC0.EC0.VPC0.DYTC 0x0013B001");
    }
  };

  auto tab_container = Container::Tab(
      {
          Radiobox(&tab_1_entries, &tab_1_selected, tab1),
          Radiobox(&tab_2_entries, &tab_2_selected, tab2),
          Radiobox(&tab_3_entries, &tab_3_selected, tab3),  // TODO add the
          /* function to disable the mouse  */
      },
      &tab_selected);

  auto container = Container::Horizontal({
      tab_menu,
      tab_container,
  });

  auto renderer = Renderer(container, [&] {
    return hbox({
               tab_menu->Render(),
               separator(),
               tab_container->Render(),

           }) |
           border;
  });

  auto screen = ScreenInteractive::TerminalOutput();
  screen.Loop(renderer);
}
