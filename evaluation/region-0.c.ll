; ModuleID = 'region-0.c'
source_filename = "region-0.c"
target datalayout = "e-m:e-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128"
target triple = "aarch64"

; Function Attrs: nofree norecurse nounwind optsize
define dso_local void @example(i32* nocapture %A, i32* nocapture readonly %B, i32* nocapture readonly %C) local_unnamed_addr #0 {
entry:
  %0 = load i32, i32* %B, align 4, !tbaa !6
  %cmp = icmp sgt i32 %0, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %1 = load i32, i32* %C, align 4, !tbaa !6
  %mul = mul nsw i32 %1, %0
  store i32 %mul, i32* %A, align 4, !tbaa !6
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %arrayidx4 = getelementptr inbounds i32, i32* %B, i64 1
  %2 = load i32, i32* %arrayidx4, align 4, !tbaa !6
  %cmp5 = icmp sgt i32 %2, 0
  br i1 %cmp5, label %if.then6, label %if.end11

if.then6:                                         ; preds = %if.end
  %arrayidx8 = getelementptr inbounds i32, i32* %C, i64 1
  %3 = load i32, i32* %arrayidx8, align 4, !tbaa !6
  %mul9 = mul nsw i32 %3, %2
  %arrayidx10 = getelementptr inbounds i32, i32* %A, i64 1
  store i32 %mul9, i32* %arrayidx10, align 4, !tbaa !6
  br label %if.end11

if.end11:                                         ; preds = %if.then6, %if.end
  ret void
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
!7 = !{!"int", !8, i64 0}
!8 = !{!"omnipotent char", !9, i64 0}
!9 = !{!"Simple C/C++ TBAA"}
