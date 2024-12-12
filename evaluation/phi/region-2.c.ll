; ModuleID = 'region-2.c'
source_filename = "region-2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind optsize uwtable
define dso_local void @example(float* nocapture readonly %A, float* nocapture readonly %B) local_unnamed_addr #0 {
entry:
  %0 = load float, float* %A, align 4, !tbaa !2
  %1 = load float, float* %B, align 4, !tbaa !2
  %cmp = fcmp ogt float %1, 0.000000e+00
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %call = tail call float @foo(float %1) #2
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %v.0 = phi float [ %call, %if.then ], [ %0, %entry ]
  tail call void @dummy(float %v.0) #2
  %arrayidx3 = getelementptr inbounds float, float* %A, i64 1
  %2 = load float, float* %arrayidx3, align 4, !tbaa !2
  %arrayidx4 = getelementptr inbounds float, float* %B, i64 1
  %3 = load float, float* %arrayidx4, align 4, !tbaa !2
  %cmp5 = fcmp ogt float %3, 0.000000e+00
  br i1 %cmp5, label %if.then6, label %if.end9

if.then6:                                         ; preds = %if.end
  %call8 = tail call float @foo(float %3) #2
  br label %if.end9

if.end9:                                          ; preds = %if.then6, %if.end
  %v.1 = phi float [ %call8, %if.then6 ], [ %2, %if.end ]
  tail call void @dummy(float %v.1) #2
  %arrayidx10 = getelementptr inbounds float, float* %A, i64 2
  %4 = load float, float* %arrayidx10, align 4, !tbaa !2
  %arrayidx11 = getelementptr inbounds float, float* %B, i64 2
  %5 = load float, float* %arrayidx11, align 4, !tbaa !2
  %cmp12 = fcmp ogt float %5, 0.000000e+00
  br i1 %cmp12, label %if.then13, label %if.end16

if.then13:                                        ; preds = %if.end9
  %call15 = tail call float @foo(float %5) #2
  br label %if.end16

if.end16:                                         ; preds = %if.then13, %if.end9
  %v.2 = phi float [ %call15, %if.then13 ], [ %4, %if.end9 ]
  tail call void @dummy(float %v.2) #2
  ret void
}

; Function Attrs: optsize
declare dso_local float @foo(float) local_unnamed_addr #1

; Function Attrs: optsize
declare dso_local void @dummy(float) local_unnamed_addr #1

attributes #0 = { nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { optsize "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind optsize }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (git@github.com:rcorcs/llvm-project.git 4021fe186d74ee73556499a0577f897126bd4b6d)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"float", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
