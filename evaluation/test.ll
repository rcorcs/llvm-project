; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local i32 @addSqred(i32 %a, i32 %b) local_unnamed_addr #0 {
entry:
  %mul = mul nsw i32 %a, %a
  %mul1 = mul nsw i32 %b, %b
  %add = add nuw nsw i32 %mul1, %mul
  ret i32 %add
}

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local i32 @divBy(i32 %a, i32 %b) local_unnamed_addr #0 {
entry:
  %div = sdiv i32 %a, %b
  ret i32 %div
}

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local i32 @foo(i32 %a, i32 %b, i32 %x) local_unnamed_addr #0 {
entry:
  %call = call i32 @addSqred(i32 %a, i32 %a)
  %add = add nsw i32 %call, %b
  %call1 = call i32 @divBy(i32 %add, i32 2)
  %call2 = call i32 @addSqred(i32 %x, i32 %x)
  %add3 = add nsw i32 %call2, 4
  %call4 = call i32 @divBy(i32 %add3, i32 2)
  %add5 = add nsw i32 %call4, %call1
  ret i32 %add5
}

attributes #0 = { norecurse nounwind readnone uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (git@github.com:rcorcs/llvm-project.git 3efd92d07126fb1b0c0553012606047307183e1e)"}
