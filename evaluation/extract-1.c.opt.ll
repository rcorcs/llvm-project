; ModuleID = 'extract-1.c.ll'
source_filename = "extract-1.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind optsize uwtable
define dso_local void @example(i32* nocapture readonly %A, i32* nocapture readonly %B) local_unnamed_addr #0 {
entry:
  br label %rolled.reg.pre

if.end16:                                         ; preds = %rolled.reg.exit
  tail call void @uses(i32 %14, i32 %16, i32 %18) #2
  ret void

rolled.reg.pre:                                   ; preds = %entry
  %0 = alloca i32, i8 3, align 4
  br label %rolled.reg.loop

rolled.reg.loop:                                  ; preds = %rolled.reg.pre, %rolled.reg.latch
  %1 = phi i8 [ 0, %rolled.reg.pre ], [ %11, %rolled.reg.latch ]
  br label %rolled.reg.bb

rolled.reg.bb:                                    ; preds = %rolled.reg.loop
  %2 = zext i8 %1 to i64
  %3 = getelementptr i32, i32* %A, i64 %2
  %4 = load i32, i32* %3, align 4
  %5 = getelementptr i32, i32* %0, i8 %1
  store i32 %4, i32* %5, align 4
  %6 = getelementptr i32, i32* %B, i64 %2
  %7 = load i32, i32* %6, align 4
  %8 = icmp sgt i32 %7, 0
  br i1 %8, label %rolled.reg.bb1, label %rolled.reg.bb2

rolled.reg.bb1:                                   ; preds = %rolled.reg.bb
  %9 = tail call i32 @foo(i32 %7) #2
  br label %rolled.reg.bb2

rolled.reg.bb2:                                   ; preds = %rolled.reg.bb1, %rolled.reg.bb
  %10 = phi i32 [ %9, %rolled.reg.bb1 ], [ %4, %rolled.reg.bb ]
  tail call void @dummy(i32 %10) #2
  br label %rolled.reg.latch

rolled.reg.latch:                                 ; preds = %rolled.reg.bb2
  %11 = add i8 %1, 1
  %12 = icmp ne i8 %11, 4
  br i1 %12, label %rolled.reg.loop, label %rolled.reg.exit

rolled.reg.exit:                                  ; preds = %rolled.reg.latch
  %13 = getelementptr i32, i32* %0, i8 0
  %14 = load i32, i32* %13, align 4
  %15 = getelementptr i32, i32* %0, i8 1
  %16 = load i32, i32* %15, align 4
  %17 = getelementptr i32, i32* %0, i8 2
  %18 = load i32, i32* %17, align 4
  br label %if.end16
}

; Function Attrs: optsize
declare dso_local i32 @foo(i32) local_unnamed_addr #1

; Function Attrs: optsize
declare dso_local void @dummy(i32) local_unnamed_addr #1

; Function Attrs: optsize
declare dso_local void @uses(i32, i32, i32) local_unnamed_addr #1

attributes #0 = { nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { optsize "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind optsize }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (git@github.com:rcorcs/llvm-project.git 4021fe186d74ee73556499a0577f897126bd4b6d)"}
