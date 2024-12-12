; ModuleID = 'example-3.c.ll'
source_filename = "example-3.c"
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "aarch64"

; Function Attrs: nofree norecurse nounwind optsize
define dso_local float @example(float* nocapture %A, float* nocapture readonly %B, float %factor, float %sum) local_unnamed_addr #0 {
entry:
  %0 = load float, float* %B, align 4, !tbaa !6
  %cmp = fcmp ogt float %0, 0.000000e+00
  br i1 %cmp, label %if.then, label %entry.if.end_crit_edge

entry.if.end_crit_edge:                           ; preds = %entry
  %.pre = load float, float* %A, align 4, !tbaa !6
  br label %if.end

if.then:                                          ; preds = %entry
  %mul = fmul float %0, %factor
  store float %mul, float* %A, align 4, !tbaa !6
  br label %if.end

if.end:                                           ; preds = %if.then, %entry.if.end_crit_edge
  %1 = phi float [ %.pre, %entry.if.end_crit_edge ], [ %mul, %if.then ]
  %arrayidx4 = getelementptr inbounds float, float* %B, i64 1
  %2 = load float, float* %arrayidx4, align 4, !tbaa !6
  %cmp5 = fcmp ogt float %2, 0.000000e+00
  br i1 %cmp5, label %if.then6, label %if.end.if.end10_crit_edge

if.end.if.end10_crit_edge:                        ; preds = %if.end
  %arrayidx11.phi.trans.insert = getelementptr inbounds float, float* %A, i64 1
  %.pre55 = load float, float* %arrayidx11.phi.trans.insert, align 4, !tbaa !6
  br label %if.end10

if.then6:                                         ; preds = %if.end
  %mul8 = fmul float %2, %factor
  %arrayidx9 = getelementptr inbounds float, float* %A, i64 1
  store float %mul8, float* %arrayidx9, align 4, !tbaa !6
  br label %if.end10

if.end10:                                         ; preds = %if.then6, %if.end.if.end10_crit_edge
  %3 = phi float [ %.pre55, %if.end.if.end10_crit_edge ], [ %mul8, %if.then6 ]
  %arrayidx13 = getelementptr inbounds float, float* %B, i64 2
  %4 = load float, float* %arrayidx13, align 4, !tbaa !6
  %cmp14 = fcmp ogt float %4, 0.000000e+00
  br i1 %cmp14, label %if.then15, label %if.end10.if.end19_crit_edge

if.end10.if.end19_crit_edge:                      ; preds = %if.end10
  %arrayidx20.phi.trans.insert = getelementptr inbounds float, float* %A, i64 2
  %.pre56 = load float, float* %arrayidx20.phi.trans.insert, align 4, !tbaa !6
  br label %if.end19

if.then15:                                        ; preds = %if.end10
  %mul17 = fmul float %4, %factor
  %arrayidx18 = getelementptr inbounds float, float* %A, i64 2
  store float %mul17, float* %arrayidx18, align 4, !tbaa !6
  br label %if.end19

if.end19:                                         ; preds = %if.then15, %if.end10.if.end19_crit_edge
  %5 = phi float [ %.pre56, %if.end10.if.end19_crit_edge ], [ %mul17, %if.then15 ]
  %arrayidx22 = getelementptr inbounds float, float* %B, i64 3
  %6 = load float, float* %arrayidx22, align 4, !tbaa !6
  %cmp23 = fcmp ogt float %6, 0.000000e+00
  br i1 %cmp23, label %if.then24, label %if.end19.if.end28_crit_edge

if.end19.if.end28_crit_edge:                      ; preds = %if.end19
  %arrayidx29.phi.trans.insert = getelementptr inbounds float, float* %A, i64 3
  %.pre57 = load float, float* %arrayidx29.phi.trans.insert, align 4, !tbaa !6
  br label %if.end28

if.then24:                                        ; preds = %if.end19
  %mul26 = fmul float %6, %factor
  %arrayidx27 = getelementptr inbounds float, float* %A, i64 3
  store float %mul26, float* %arrayidx27, align 4, !tbaa !6
  br label %if.end28

if.end28:                                         ; preds = %if.then24, %if.end19.if.end28_crit_edge
  %7 = phi float [ %.pre57, %if.end19.if.end28_crit_edge ], [ %mul26, %if.then24 ]
  %add = fadd float %1, %sum
  %add12 = fadd float %add, %3
  %add21 = fadd float %add12, %5
  %add30 = fadd float %add21, %7
  ret float %add30
}

attributes #0 = { nofree norecurse nounwind optsize "disable-tail-calls"="false" "frame-pointer"="non-leaf" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="generic" "target-features"="+neon" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"branch-target-enforcement", i32 0}
!2 = !{i32 1, !"sign-return-address", i32 0}
!3 = !{i32 1, !"sign-return-address-all", i32 0}
!4 = !{i32 1, !"sign-return-address-with-bkey", i32 0}
!5 = !{!"clang version 12.0.0 (git@github.com:rcorcs/llvm-project.git 4021fe186d74ee73556499a0577f897126bd4b6d)"}
!6 = !{!7, !7, i64 0}
!7 = !{!"float", !8, i64 0}
!8 = !{!"omnipotent char", !9, i64 0}
!9 = !{!"Simple C/C++ TBAA"}
