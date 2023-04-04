; ModuleID = 'tsvc-271.c'
source_filename = "tsvc-271.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx13.0.0"

; Function Attrs: nofree norecurse nounwind optsize ssp uwtable
define void @test(float* nocapture %a, float* nocapture readonly %b, float* nocapture readonly %c) local_unnamed_addr #0 {
entry:
  %0 = load float, float* %b, align 4, !tbaa !7
  %cmp = fcmp ogt float %0, 0.000000e+00
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %1 = load float, float* %c, align 4, !tbaa !7
  %mul = fmul float %0, %1
  %2 = load float, float* %a, align 4, !tbaa !7
  %add = fadd float %2, %mul
  store float %add, float* %a, align 4, !tbaa !7
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %arrayidx4 = getelementptr inbounds float, float* %b, i64 1
  %3 = load float, float* %arrayidx4, align 4, !tbaa !7
  %cmp5 = fcmp ogt float %3, 0.000000e+00
  br i1 %cmp5, label %if.then6, label %if.end12

if.then6:                                         ; preds = %if.end
  %arrayidx8 = getelementptr inbounds float, float* %c, i64 1
  %4 = load float, float* %arrayidx8, align 4, !tbaa !7
  %mul9 = fmul float %3, %4
  %arrayidx10 = getelementptr inbounds float, float* %a, i64 1
  %5 = load float, float* %arrayidx10, align 4, !tbaa !7
  %add11 = fadd float %5, %mul9
  store float %add11, float* %arrayidx10, align 4, !tbaa !7
  br label %if.end12

if.end12:                                         ; preds = %if.then6, %if.end
  %arrayidx13 = getelementptr inbounds float, float* %b, i64 2
  %6 = load float, float* %arrayidx13, align 4, !tbaa !7
  %cmp14 = fcmp ogt float %6, 0.000000e+00
  br i1 %cmp14, label %if.then15, label %if.end21

if.then15:                                        ; preds = %if.end12
  %arrayidx17 = getelementptr inbounds float, float* %c, i64 2
  %7 = load float, float* %arrayidx17, align 4, !tbaa !7
  %mul18 = fmul float %6, %7
  %arrayidx19 = getelementptr inbounds float, float* %a, i64 2
  %8 = load float, float* %arrayidx19, align 4, !tbaa !7
  %add20 = fadd float %8, %mul18
  store float %add20, float* %arrayidx19, align 4, !tbaa !7
  br label %if.end21

if.end21:                                         ; preds = %if.then15, %if.end12
  %arrayidx22 = getelementptr inbounds float, float* %b, i64 3
  %9 = load float, float* %arrayidx22, align 4, !tbaa !7
  %cmp23 = fcmp ogt float %9, 0.000000e+00
  br i1 %cmp23, label %if.then24, label %if.end30

if.then24:                                        ; preds = %if.end21
  %arrayidx26 = getelementptr inbounds float, float* %c, i64 3
  %10 = load float, float* %arrayidx26, align 4, !tbaa !7
  %mul27 = fmul float %9, %10
  %arrayidx28 = getelementptr inbounds float, float* %a, i64 3
  %11 = load float, float* %arrayidx28, align 4, !tbaa !7
  %add29 = fadd float %11, %mul27
  store float %add29, float* %arrayidx28, align 4, !tbaa !7
  br label %if.end30

if.end30:                                         ; preds = %if.then24, %if.end21
  ret void
}

attributes #0 = { nofree norecurse nounwind optsize ssp uwtable "disable-tail-calls"="false" "frame-pointer"="non-leaf" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-a12" "target-features"="+aes,+crc,+crypto,+fp-armv8,+fullfp16,+lse,+neon,+ras,+rcpc,+rdm,+sha2,+v8.3a,+zcm,+zcz" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5}
!llvm.ident = !{!6}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 1, !"branch-target-enforcement", i32 0}
!2 = !{i32 1, !"sign-return-address", i32 0}
!3 = !{i32 1, !"sign-return-address-all", i32 0}
!4 = !{i32 1, !"sign-return-address-with-bkey", i32 0}
!5 = !{i32 7, !"PIC Level", i32 2}
!6 = !{!"clang version 12.0.0 (git@github.com:rcorcs/llvm-project.git c9e4bb0894674957820c010c4ad5897e9a15f0b6)"}
!7 = !{!8, !8, i64 0}
!8 = !{!"float", !9, i64 0}
!9 = !{!"omnipotent char", !10, i64 0}
!10 = !{!"Simple C/C++ TBAA"}
