#include <cstdio>
#include <cstdlib>
#include <memory>  // for allocator, __shared_ptr_access, shared_ptr
#include <string>  // for wstring, basic_string
#include <vector>  // for vector
#include <fstream>
#include <iostream>

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Radiobox, Horizontal, Menu, Renderer, Tab
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for Element, separator, hbox, operator|, border

using namespace ftxui;
char conservationStr[80];
char fnLockStr[80];

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
    std::string ret{ std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>() };
    ifs.close(); // must close the inout stream so the file can be cleaned up
    if (std::remove("temp.txt") != 0) {
        perror("Error deleting temporary file");
    }
    return std::stoi(ret);
}

int main(int argc, const char* argv[]) {
  int value = 50;
  std::vector<std::wstring> tab_values{
      L"Conservation Mode",
      L"fn_lock",
      L"Mouse Lock",
  };
  int tab_selected = 0;
  auto tab_menu = Menu(&tab_values, &tab_selected);

  std::vector<std::wstring> tab_1_entries{
      L"Off",
      L"On",
  };
  int tab_1_selected = execute("cat /sys/bus/platform/drivers/ideapad_acpi/VPC2004:00/conservation_mode");

  std::vector<std::wstring> tab_2_entries{
      L"On",
      L"Off",
  };

  /* int tab_2_selected = system("cat /sys/bus/platform/drivers/ideapad_acpi/VPC2004:00/fn_lock"); */
  int tab_2_selected = execute("cat /sys/bus/platform/drivers/ideapad_acpi/VPC2004:00/fn_lock");
  std::vector<std::wstring> tab_3_entries{
      L"Off",
      L"On",
  };
  int tab_3_selected = 0;
  auto tab_container = Container::Tab(
      {
          Radiobox(&tab_1_entries, &tab_1_selected,
                   [&] {
                     sprintf(conservationStr,
                             "echo %d > "
                             "/sys/bus/platform/drivers/ideapad_acpi/"
                             "VPC2004:00/conservation_mode",
                             tab_1_selected);
                     system(conservationStr);
                   }),
          Radiobox(&tab_2_entries, &tab_2_selected,
                   [&] {
                     sprintf(fnLockStr,
                             "echo %d > "
                             "/sys/bus/platform/drivers/ideapad_acpi/"
                             "VPC2004:00/fn_lock",
                             tab_2_selected);
                     system(fnLockStr);
                   }),
          Radiobox(&tab_3_entries, &tab_3_selected, [&] { }), //TODO add the function to disable the mouse 
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
