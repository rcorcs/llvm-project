; ModuleID = 'tsvc-273.c.ll'
source_filename = "tsvc-273.c"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128"
target triple = "arm64-apple-macosx13.0.0"

; Function Attrs: nofree norecurse nounwind optsize ssp uwtable
define void @test(float* nocapture %a, float* nocapture %b, float* nocapture %c, float* nocapture readonly %d, float* nocapture readonly %e) local_unnamed_addr #0 {
entry:
  %0 = load float, float* %d, align 4, !tbaa !7
  %1 = load float, float* %e, align 4, !tbaa !7
  %mul = fmul float %0, %1
  %2 = load float, float* %a, align 4, !tbaa !7
  %add = fadd float %2, %mul
  store float %add, float* %a, align 4, !tbaa !7
  %cmp = fcmp olt float %add, 0.000000e+00
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %3 = load float, float* %d, align 4, !tbaa !7
  %4 = load float, float* %e, align 4, !tbaa !7
  %mul6 = fmul float %3, %4
  %5 = load float, float* %b, align 4, !tbaa !7
  %add8 = fadd float %5, %mul6
  store float %add8, float* %b, align 4, !tbaa !7
  %.pre = load float, float* %a, align 4, !tbaa !7
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %6 = phi float [ %.pre, %if.then ], [ %add, %entry ]
  %7 = load float, float* %d, align 4, !tbaa !7
  %mul11 = fmul float %6, %7
  %8 = load float, float* %c, align 4, !tbaa !7
  %add13 = fadd float %8, %mul11
  store float %add13, float* %c, align 4, !tbaa !7
  %arrayidx14 = getelementptr inbounds float, float* %d, i64 1
  %9 = load float, float* %arrayidx14, align 4, !tbaa !7
  %arrayidx15 = getelementptr inbounds float, float* %e, i64 1
  %10 = load float, float* %arrayidx15, align 4, !tbaa !7
  %mul16 = fmul float %9, %10
  %arrayidx17 = getelementptr inbounds float, float* %a, i64 1
  %11 = load float, float* %arrayidx17, align 4, !tbaa !7
  %add18 = fadd float %11, %mul16
  store float %add18, float* %arrayidx17, align 4, !tbaa !7
  %cmp20 = fcmp olt float %add18, 0.000000e+00
  br i1 %cmp20, label %if.then21, label %if.end27

if.then21:                                        ; preds = %if.end
  %12 = load float, float* %arrayidx14, align 4, !tbaa !7
  %13 = load float, float* %arrayidx15, align 4, !tbaa !7
  %mul24 = fmul float %12, %13
  %arrayidx25 = getelementptr inbounds float, float* %b, i64 1
  %14 = load float, float* %arrayidx25, align 4, !tbaa !7
  %add26 = fadd float %14, %mul24
  store float %add26, float* %arrayidx25, align 4, !tbaa !7
  %.pre106 = load float, float* %arrayidx17, align 4, !tbaa !7
  br label %if.end27

if.end27:                                         ; preds = %if.then21, %if.end
  %15 = phi float [ %.pre106, %if.then21 ], [ %add18, %if.end ]
  %16 = load float, float* %arrayidx14, align 4, !tbaa !7
  %mul30 = fmul float %15, %16
  %arrayidx31 = getelementptr inbounds float, float* %c, i64 1
  %17 = load float, float* %arrayidx31, align 4, !tbaa !7
  %add32 = fadd float %17, %mul30
  store float %add32, float* %arrayidx31, align 4, !tbaa !7
  %arrayidx33 = getelementptr inbounds float, float* %d, i64 2
  %18 = load float, float* %arrayidx33, align 4, !tbaa !7
  %arrayidx34 = getelementptr inbounds float, float* %e, i64 2
  %19 = load float, float* %arrayidx34, align 4, !tbaa !7
  %mul35 = fmul float %18, %19
  %arrayidx36 = getelementptr inbounds float, float* %a, i64 2
  %20 = load float, float* %arrayidx36, align 4, !tbaa !7
  %add37 = fadd float %20, %mul35
  store float %add37, float* %arrayidx36, align 4, !tbaa !7
  %cmp39 = fcmp olt float %add37, 0.000000e+00
  br i1 %cmp39, label %if.then40, label %if.end46

if.then40:                                        ; preds = %if.end27
  %21 = load float, float* %arrayidx33, align 4, !tbaa !7
  %22 = load float, float* %arrayidx34, align 4, !tbaa !7
  %mul43 = fmul float %21, %22
  %arrayidx44 = getelementptr inbounds float, float* %b, i64 2
  %23 = load float, float* %arrayidx44, align 4, !tbaa !7
  %add45 = fadd float %23, %mul43
  store float %add45, float* %arrayidx44, align 4, !tbaa !7
  %.pre107 = load float, float* %arrayidx36, align 4, !tbaa !7
  br label %if.end46

if.end46:                                         ; preds = %if.then40, %if.end27
  %24 = phi float [ %.pre107, %if.then40 ], [ %add37, %if.end27 ]
  %25 = load float, float* %arrayidx33, align 4, !tbaa !7
  %mul49 = fmul float %24, %25
  %arrayidx50 = getelementptr inbounds float, float* %c, i64 2
  %26 = load float, float* %arrayidx50, align 4, !tbaa !7
  %add51 = fadd float %26, %mul49
  store float %add51, float* %arrayidx50, align 4, !tbaa !7
  %arrayidx52 = getelementptr inbounds float, float* %d, i64 3
  %27 = load float, float* %arrayidx52, align 4, !tbaa !7
  %arrayidx53 = getelementptr inbounds float, float* %e, i64 3
  %28 = load float, float* %arrayidx53, align 4, !tbaa !7
  %mul54 = fmul float %27, %28
  %arrayidx55 = getelementptr inbounds float, float* %a, i64 3
  %29 = load float, float* %arrayidx55, align 4, !tbaa !7
  %add56 = fadd float %29, %mul54
  store float %add56, float* %arrayidx55, align 4, !tbaa !7
  %cmp58 = fcmp olt float %add56, 0.000000e+00
  br i1 %cmp58, label %if.then59, label %if.end65

if.then59:                                        ; preds = %if.end46
  %30 = load float, float* %arrayidx52, align 4, !tbaa !7
  %31 = load float, float* %arrayidx53, align 4, !tbaa !7
  %mul62 = fmul float %30, %31
  %arrayidx63 = getelementptr inbounds float, float* %b, i64 3
  %32 = load float, float* %arrayidx63, align 4, !tbaa !7
  %add64 = fadd float %32, %mul62
  store float %add64, float* %arrayidx63, align 4, !tbaa !7
  %.pre108 = load float, float* %arrayidx55, align 4, !tbaa !7
  br label %if.end65

if.end65:                                         ; preds = %if.then59, %if.end46
  %33 = phi float [ %.pre108, %if.then59 ], [ %add56, %if.end46 ]
  %34 = load float, float* %arrayidx52, align 4, !tbaa !7
  %mul68 = fmul float %33, %34
  %arrayidx69 = getelementptr inbounds float, float* %c, i64 3
  %35 = load float, float* %arrayidx69, align 4, !tbaa !7
  %add70 = fadd float %35, %mul68
  store float %add70, float* %arrayidx69, align 4, !tbaa !7
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
!6 = !{!"clang version 12.0.0 (git@github.com:rcorcs/llvm-project.git 5426f421a25f8a1e8a9861e4cb3a8cad0cd30674)"}
!7 = !{!8, !8, i64 0}
!8 = !{!"float", !9, i64 0}
!9 = !{!"omnipotent char", !10, i64 0}
!10 = !{!"Simple C/C++ TBAA"}
