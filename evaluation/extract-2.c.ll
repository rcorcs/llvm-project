; ModuleID = 'extract-2.c'
source_filename = "extract-2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind optsize uwtable
define dso_local void @example(i32* nocapture readonly %A, i32* nocapture readonly %B) local_unnamed_addr #0 {
entry:
  %0 = load i32, i32* %A, align 4, !tbaa !2
  %1 = load i32, i32* %B, align 4, !tbaa !2
  %cmp = icmp sgt i32 %1, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %call = tail call i32 @foo(i32 %1) #2
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %v0.0 = phi i32 [ %call, %if.then ], [ %0, %entry ]
  tail call void @dummy(i32 %v0.0) #2
  %arrayidx3 = getelementptr inbounds i32, i32* %A, i64 1
  %2 = load i32, i32* %arrayidx3, align 4, !tbaa !2
  %arrayidx4 = getelementptr inbounds i32, i32* %B, i64 1
  %3 = load i32, i32* %arrayidx4, align 4, !tbaa !2
  %cmp5 = icmp sgt i32 %3, 0
  br i1 %cmp5, label %if.then6, label %if.end9

if.then6:                                         ; preds = %if.end
  %call8 = tail call i32 @foo(i32 %3) #2
  br label %if.end9

if.end9:                                          ; preds = %if.then6, %if.end
  %v1.0 = phi i32 [ %call8, %if.then6 ], [ %2, %if.end ]
  tail call void @dummy(i32 %v1.0) #2
  %arrayidx10 = getelementptr inbounds i32, i32* %A, i64 2
  %4 = load i32, i32* %arrayidx10, align 4, !tbaa !2
  %arrayidx11 = getelementptr inbounds i32, i32* %B, i64 2
  %5 = load i32, i32* %arrayidx11, align 4, !tbaa !2
  %cmp12 = icmp sgt i32 %5, 0
  br i1 %cmp12, label %if.then13, label %if.end16

if.then13:                                        ; preds = %if.end9
  %call15 = tail call i32 @foo(i32 %5) #2
  br label %if.end16

if.end16:                                         ; preds = %if.then13, %if.end9
  %v2.0 = phi i32 [ %call15, %if.then13 ], [ %4, %if.end9 ]
  tail call void @dummy(i32 %v2.0) #2
  tail call void @uses(i32 %v0.0, i32 %v1.0, i32 %v2.0) #2
  ret void
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
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
