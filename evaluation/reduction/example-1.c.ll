; ModuleID = 'example-1.c'
source_filename = "example-1.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind optsize readonly uwtable
define dso_local float @example(float* nocapture readonly %A, float %max) local_unnamed_addr #0 {
entry:
  %0 = load float, float* %A, align 4, !tbaa !2
  %1 = tail call float @llvm.fabs.f32(float %0)
  %cmp = fcmp ogt float %1, %max
  %max.addr.0 = select i1 %cmp, float %1, float %max
  %arrayidx2 = getelementptr inbounds float, float* %A, i64 1
  %2 = load float, float* %arrayidx2, align 4, !tbaa !2
  %3 = tail call float @llvm.fabs.f32(float %2)
  %cmp3 = fcmp ogt float %3, %max.addr.0
  %max.addr.1 = select i1 %cmp3, float %3, float %max.addr.0
  %arrayidx7 = getelementptr inbounds float, float* %A, i64 2
  %4 = load float, float* %arrayidx7, align 4, !tbaa !2
  %5 = tail call float @llvm.fabs.f32(float %4)
  %cmp8 = fcmp ogt float %5, %max.addr.1
  %max.addr.2 = select i1 %cmp8, float %5, float %max.addr.1
  %arrayidx12 = getelementptr inbounds float, float* %A, i64 3
  %6 = load float, float* %arrayidx12, align 4, !tbaa !2
  %7 = tail call float @llvm.fabs.f32(float %6)
  %cmp13 = fcmp ogt float %7, %max.addr.2
  %max.addr.3 = select i1 %cmp13, float %7, float %max.addr.2
  ret float %max.addr.3
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare float @llvm.fabs.f32(float) #1

attributes #0 = { nounwind optsize readonly uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (git@github.com:rcorcs/llvm-project.git 58c4671bdb03c06ebb7ef2606972c03b53d6f07f)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"float", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
