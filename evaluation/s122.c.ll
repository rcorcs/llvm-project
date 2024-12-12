; ModuleID = 's122.c'
source_filename = "s122.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind optsize uwtable
define dso_local void @s122(i32 %iterations, i32 %nl, i32 %n1, i32 %n3, float* %a, float* %b) local_unnamed_addr #0 {
entry:
  %cmp26 = icmp sgt i32 %iterations, 0
  br i1 %cmp26, label %for.body.lr.ph, label %for.cond.cleanup

for.body.lr.ph:                                   ; preds = %entry
  %cmp323 = icmp slt i32 %n1, 32001
  %0 = add i32 %n1, -1
  %1 = sext i32 %0 to i64
  %2 = sext i32 %n3 to i64
  %3 = add i32 %n3, %n1
  %4 = add i32 %3, -1
  %5 = icmp sgt i32 %4, 32000
  %smax = select i1 %5, i32 %4, i32 32000
  %6 = sub i32 %smax, %n1
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4, %entry
  ret void

for.body:                                         ; preds = %for.body.lr.ph, %for.cond.cleanup4
  %nl1.027 = phi i32 [ 0, %for.body.lr.ph ], [ %inc, %for.cond.cleanup4 ]
  br i1 %cmp323, label %for.body5.preheader, label %for.cond.cleanup4

for.body5.preheader:                              ; preds = %for.body
  %7 = udiv i32 %6, %n3
  %8 = add i32 %7, 1
  %xtraiter = and i32 %8, 7
  %9 = icmp ult i32 %7, 7
  br i1 %9, label %for.cond.cleanup4.loopexit.unr-lcssa, label %for.body5.preheader.new

for.body5.preheader.new:                          ; preds = %for.body5.preheader
  %unroll_iter = and i32 %8, -8
  br label %for.body5

for.cond.cleanup4.loopexit.unr-lcssa:             ; preds = %for.body5, %for.body5.preheader
  %indvars.iv28.unr = phi i64 [ 0, %for.body5.preheader ], [ %indvars.iv.next29.7, %for.body5 ]
  %indvars.iv.unr = phi i64 [ %1, %for.body5.preheader ], [ %indvars.iv.next.7, %for.body5 ]
  %lcmp.mod.not = icmp eq i32 %xtraiter, 0
  br i1 %lcmp.mod.not, label %for.cond.cleanup4, label %for.body5.epil

for.body5.epil:                                   ; preds = %for.cond.cleanup4.loopexit.unr-lcssa, %for.body5.epil
  %indvars.iv28.epil = phi i64 [ %indvars.iv.next29.epil, %for.body5.epil ], [ %indvars.iv28.unr, %for.cond.cleanup4.loopexit.unr-lcssa ]
  %indvars.iv.epil = phi i64 [ %indvars.iv.next.epil, %for.body5.epil ], [ %indvars.iv.unr, %for.cond.cleanup4.loopexit.unr-lcssa ]
  %epil.iter = phi i32 [ %epil.iter.sub, %for.body5.epil ], [ %xtraiter, %for.cond.cleanup4.loopexit.unr-lcssa ]
  %indvars.iv.next29.epil = add nuw nsw i64 %indvars.iv28.epil, 1
  %10 = sub nsw i64 31999, %indvars.iv28.epil
  %arrayidx.epil = getelementptr inbounds float, float* %b, i64 %10
  %11 = load float, float* %arrayidx.epil, align 4, !tbaa !2
  %arrayidx8.epil = getelementptr inbounds float, float* %a, i64 %indvars.iv.epil
  %12 = load float, float* %arrayidx8.epil, align 4, !tbaa !2
  %add9.epil = fadd float %11, %12
  store float %add9.epil, float* %arrayidx8.epil, align 4, !tbaa !2
  %indvars.iv.next.epil = add i64 %indvars.iv.epil, %2
  %epil.iter.sub = add i32 %epil.iter, -1
  %epil.iter.cmp.not = icmp eq i32 %epil.iter.sub, 0
  br i1 %epil.iter.cmp.not, label %for.cond.cleanup4, label %for.body5.epil, !llvm.loop !6

for.cond.cleanup4:                                ; preds = %for.body5.epil, %for.cond.cleanup4.loopexit.unr-lcssa, %for.body
  tail call void @dummy(float* %a, float* %b) #2
  %inc = add nuw nsw i32 %nl1.027, 1
  %exitcond.not = icmp eq i32 %inc, %iterations
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.body

for.body5:                                        ; preds = %for.body5, %for.body5.preheader.new
  %indvars.iv28 = phi i64 [ 0, %for.body5.preheader.new ], [ %indvars.iv.next29.7, %for.body5 ]
  %indvars.iv = phi i64 [ %1, %for.body5.preheader.new ], [ %indvars.iv.next.7, %for.body5 ]
  %niter = phi i32 [ %unroll_iter, %for.body5.preheader.new ], [ %niter.nsub.7, %for.body5 ]
  %13 = sub nsw i64 31999, %indvars.iv28
  %arrayidx = getelementptr inbounds float, float* %b, i64 %13
  %14 = load float, float* %arrayidx, align 4, !tbaa !2
  %arrayidx8 = getelementptr inbounds float, float* %a, i64 %indvars.iv
  %15 = load float, float* %arrayidx8, align 4, !tbaa !2
  %add9 = fadd float %14, %15
  store float %add9, float* %arrayidx8, align 4, !tbaa !2
  %indvars.iv.next = add i64 %indvars.iv, %2
  %16 = sub nsw i64 31998, %indvars.iv28
  %arrayidx.1 = getelementptr inbounds float, float* %b, i64 %16
  %17 = load float, float* %arrayidx.1, align 4, !tbaa !2
  %arrayidx8.1 = getelementptr inbounds float, float* %a, i64 %indvars.iv.next
  %18 = load float, float* %arrayidx8.1, align 4, !tbaa !2
  %add9.1 = fadd float %17, %18
  store float %add9.1, float* %arrayidx8.1, align 4, !tbaa !2
  %indvars.iv.next.1 = add i64 %indvars.iv.next, %2
  %19 = sub nsw i64 31997, %indvars.iv28
  %arrayidx.2 = getelementptr inbounds float, float* %b, i64 %19
  %20 = load float, float* %arrayidx.2, align 4, !tbaa !2
  %arrayidx8.2 = getelementptr inbounds float, float* %a, i64 %indvars.iv.next.1
  %21 = load float, float* %arrayidx8.2, align 4, !tbaa !2
  %add9.2 = fadd float %20, %21
  store float %add9.2, float* %arrayidx8.2, align 4, !tbaa !2
  %indvars.iv.next.2 = add i64 %indvars.iv.next.1, %2
  %22 = sub nsw i64 31996, %indvars.iv28
  %arrayidx.3 = getelementptr inbounds float, float* %b, i64 %22
  %23 = load float, float* %arrayidx.3, align 4, !tbaa !2
  %arrayidx8.3 = getelementptr inbounds float, float* %a, i64 %indvars.iv.next.2
  %24 = load float, float* %arrayidx8.3, align 4, !tbaa !2
  %add9.3 = fadd float %23, %24
  store float %add9.3, float* %arrayidx8.3, align 4, !tbaa !2
  %indvars.iv.next.3 = add i64 %indvars.iv.next.2, %2
  %25 = sub nsw i64 31995, %indvars.iv28
  %arrayidx.4 = getelementptr inbounds float, float* %b, i64 %25
  %26 = load float, float* %arrayidx.4, align 4, !tbaa !2
  %arrayidx8.4 = getelementptr inbounds float, float* %a, i64 %indvars.iv.next.3
  %27 = load float, float* %arrayidx8.4, align 4, !tbaa !2
  %add9.4 = fadd float %26, %27
  store float %add9.4, float* %arrayidx8.4, align 4, !tbaa !2
  %indvars.iv.next.4 = add i64 %indvars.iv.next.3, %2
  %28 = sub nsw i64 31994, %indvars.iv28
  %arrayidx.5 = getelementptr inbounds float, float* %b, i64 %28
  %29 = load float, float* %arrayidx.5, align 4, !tbaa !2
  %arrayidx8.5 = getelementptr inbounds float, float* %a, i64 %indvars.iv.next.4
  %30 = load float, float* %arrayidx8.5, align 4, !tbaa !2
  %add9.5 = fadd float %29, %30
  store float %add9.5, float* %arrayidx8.5, align 4, !tbaa !2
  %indvars.iv.next.5 = add i64 %indvars.iv.next.4, %2
  %indvars.iv.next29.6 = or i64 %indvars.iv28, 7
  %31 = sub nsw i64 31993, %indvars.iv28
  %arrayidx.6 = getelementptr inbounds float, float* %b, i64 %31
  %32 = load float, float* %arrayidx.6, align 4, !tbaa !2
  %arrayidx8.6 = getelementptr inbounds float, float* %a, i64 %indvars.iv.next.5
  %33 = load float, float* %arrayidx8.6, align 4, !tbaa !2
  %add9.6 = fadd float %32, %33
  store float %add9.6, float* %arrayidx8.6, align 4, !tbaa !2
  %indvars.iv.next.6 = add i64 %indvars.iv.next.5, %2
  %indvars.iv.next29.7 = add nuw nsw i64 %indvars.iv28, 8
  %34 = sub nsw i64 31999, %indvars.iv.next29.6
  %arrayidx.7 = getelementptr inbounds float, float* %b, i64 %34
  %35 = load float, float* %arrayidx.7, align 4, !tbaa !2
  %arrayidx8.7 = getelementptr inbounds float, float* %a, i64 %indvars.iv.next.6
  %36 = load float, float* %arrayidx8.7, align 4, !tbaa !2
  %add9.7 = fadd float %35, %36
  store float %add9.7, float* %arrayidx8.7, align 4, !tbaa !2
  %indvars.iv.next.7 = add i64 %indvars.iv.next.6, %2
  %niter.nsub.7 = add i32 %niter, -8
  %niter.ncmp.7.not = icmp eq i32 %niter.nsub.7, 0
  br i1 %niter.ncmp.7.not, label %for.cond.cleanup4.loopexit.unr-lcssa, label %for.body5, !llvm.loop !8
}

; Function Attrs: optsize
declare dso_local void @dummy(float*, float*) local_unnamed_addr #1

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
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.unroll.disable"}
!8 = distinct !{!8, !7}
