#include "packing.h"

int main (int argc, char** argv){
  treeNode* root = Load_binary_tree_from_file(argv[1]);
  Perform_packing(root);
  Save_packing_to_file(argv[2], root);
  return 0;
}
