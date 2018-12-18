#include "prodcons.h"
#include "raii_log_func.h"

int main(int argc, char* argv[]) { RAII_LOG_FUNC;
  Prodcons prodcons(argc, argv);
  return prodcons.Run();
}
