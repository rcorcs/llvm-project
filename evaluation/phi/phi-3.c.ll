; ModuleID = 'phi-3.c'
source_filename = "phi-3.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind optsize uwtable
define dso_local void @example(i32* nocapture readnone %A, i32* nocapture readonly %B) local_unnamed_addr #0 {
entry:
  %0 = load i32, i32* %B, align 4, !tbaa !2
  %cmp = icmp sgt i32 %0, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %call = tail call i32 @foo(i32 %0) #2
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %v.0 = phi i32 [ %call, %if.then ], [ 0, %entry ]
  tail call void @dummy(i32 %v.0) #2
  %arrayidx2 = getelementptr inbounds i32, i32* %B, i64 1
  %1 = load i32, i32* %arrayidx2, align 4, !tbaa !2
  %cmp3 = icmp sgt i32 %1, 0
  br i1 %cmp3, label %if.then4, label %if.end7

if.then4:                                         ; preds = %if.end
  %call6 = tail call i32 @foo(i32 %1) #2
  br label %if.end7

if.end7:                                          ; preds = %if.then4, %if.end
  %v.1 = phi i32 [ %call6, %if.then4 ], [ 1, %if.end ]
  tail call void @dummy(i32 %v.1) #2
  %arrayidx8 = getelementptr inbounds i32, i32* %B, i64 2
  %2 = load i32, i32* %arrayidx8, align 4, !tbaa !2
  %cmp9 = icmp sgt i32 %2, 0
  br i1 %cmp9, label %if.then10, label %if.end13

if.then10:                                        ; preds = %if.end7
  %call12 = tail call i32 @foo(i32 %2) #2
  br label %if.end13

if.end13:                                         ; preds = %if.then10, %if.end7
  %v.2 = phi i32 [ %call12, %if.then10 ], [ 2, %if.end7 ]
  tail call void @dummy(i32 %v.2) #2
  ret void
}

; Function Attrs: optsize
declare dso_local i32 @foo(i32) local_unnamed_addr #1

; Function Attrs: optsize
declare dso_local void @dummy(i32) local_unnamed_addr #1

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
