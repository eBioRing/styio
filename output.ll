; ModuleID = 'styio'
source_filename = "styio"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"

declare i32 @print_cxx()

define i32 @main() {
main_entry:
  %0 = call i32 @print_cxx()
  ret i32 0
}
