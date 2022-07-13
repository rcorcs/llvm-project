; ModuleID = 'bmhsrch.c'
source_filename = "bmhsrch.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@pat = internal global i8* null, align 8
@patlen = internal global i32 0, align 4
@skip = internal global [256 x i32] zeroinitializer, align 16
@skip2 = internal global i32 0, align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @bmh_init(i8* %pattern) #0 {
entry:
  %pattern.addr = alloca i8*, align 8
  %i = alloca i32, align 4
  %lastpatchar = alloca i32, align 4
  store i8* %pattern, i8** %pattern.addr, align 8
  %0 = load i8*, i8** %pattern.addr, align 8
  store i8* %0, i8** @pat, align 8
  %1 = load i8*, i8** %pattern.addr, align 8
  %call = call i64 @strlen(i8* %1) #2
  %conv = trunc i64 %call to i32
  store i32 %conv, i32* @patlen, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %2 = load i32, i32* %i, align 4
  %cmp = icmp sle i32 %2, 255
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %3 = load i32, i32* @patlen, align 4
  %4 = load i32, i32* %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds [256 x i32], [256 x i32]* @skip, i64 0, i64 %idxprom
  store i32 %3, i32* %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %5 = load i32, i32* %i, align 4
  %inc = add nsw i32 %5, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond, !llvm.loop !2

for.end:                                          ; preds = %for.cond
  store i32 0, i32* %i, align 4
  br label %for.cond2

for.cond2:                                        ; preds = %for.inc11, %for.end
  %6 = load i32, i32* %i, align 4
  %7 = load i32, i32* @patlen, align 4
  %cmp3 = icmp slt i32 %6, %7
  br i1 %cmp3, label %for.body5, label %for.end13

for.body5:                                        ; preds = %for.cond2
  %8 = load i32, i32* @patlen, align 4
  %9 = load i32, i32* %i, align 4
  %sub = sub nsw i32 %8, %9
  %sub6 = sub nsw i32 %sub, 1
  %10 = load i8*, i8** @pat, align 8
  %11 = load i32, i32* %i, align 4
  %idxprom7 = sext i32 %11 to i64
  %arrayidx8 = getelementptr inbounds i8, i8* %10, i64 %idxprom7
  %12 = load i8, i8* %arrayidx8, align 1
  %idxprom9 = zext i8 %12 to i64
  %arrayidx10 = getelementptr inbounds [256 x i32], [256 x i32]* @skip, i64 0, i64 %idxprom9
  store i32 %sub6, i32* %arrayidx10, align 4
  br label %for.inc11

for.inc11:                                        ; preds = %for.body5
  %13 = load i32, i32* %i, align 4
  %inc12 = add nsw i32 %13, 1
  store i32 %inc12, i32* %i, align 4
  br label %for.cond2, !llvm.loop !4

for.end13:                                        ; preds = %for.cond2
  %14 = load i8*, i8** @pat, align 8
  %15 = load i32, i32* @patlen, align 4
  %sub14 = sub nsw i32 %15, 1
  %idxprom15 = sext i32 %sub14 to i64
  %arrayidx16 = getelementptr inbounds i8, i8* %14, i64 %idxprom15
  %16 = load i8, i8* %arrayidx16, align 1
  %conv17 = zext i8 %16 to i32
  store i32 %conv17, i32* %lastpatchar, align 4
  %17 = load i32, i32* %lastpatchar, align 4
  %idxprom18 = sext i32 %17 to i64
  %arrayidx19 = getelementptr inbounds [256 x i32], [256 x i32]* @skip, i64 0, i64 %idxprom18
  store i32 32767, i32* %arrayidx19, align 4
  %18 = load i32, i32* @patlen, align 4
  store i32 %18, i32* @skip2, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond20

for.cond20:                                       ; preds = %for.inc32, %for.end13
  %19 = load i32, i32* %i, align 4
  %20 = load i32, i32* @patlen, align 4
  %sub21 = sub nsw i32 %20, 1
  %cmp22 = icmp slt i32 %19, %sub21
  br i1 %cmp22, label %for.body24, label %for.end34

for.body24:                                       ; preds = %for.cond20
  %21 = load i8*, i8** @pat, align 8
  %22 = load i32, i32* %i, align 4
  %idxprom25 = sext i32 %22 to i64
  %arrayidx26 = getelementptr inbounds i8, i8* %21, i64 %idxprom25
  %23 = load i8, i8* %arrayidx26, align 1
  %conv27 = zext i8 %23 to i32
  %24 = load i32, i32* %lastpatchar, align 4
  %cmp28 = icmp eq i32 %conv27, %24
  br i1 %cmp28, label %if.then, label %if.end

if.then:                                          ; preds = %for.body24
  %25 = load i32, i32* @patlen, align 4
  %26 = load i32, i32* %i, align 4
  %sub30 = sub nsw i32 %25, %26
  %sub31 = sub nsw i32 %sub30, 1
  store i32 %sub31, i32* @skip2, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body24
  br label %for.inc32

for.inc32:                                        ; preds = %if.end
  %27 = load i32, i32* %i, align 4
  %inc33 = add nsw i32 %27, 1
  store i32 %inc33, i32* %i, align 4
  br label %for.cond20, !llvm.loop !5

for.end34:                                        ; preds = %for.cond20
  ret void
}

; Function Attrs: nounwind readonly willreturn
declare dso_local i64 @strlen(i8*) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @bmh_search(i8* %string, i32 %stringlen) #0 {
entry:
  %retval = alloca i8*, align 8
  %string.addr = alloca i8*, align 8
  %stringlen.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %s = alloca i8*, align 8
  store i8* %string, i8** %string.addr, align 8
  store i32 %stringlen, i32* %stringlen.addr, align 4
  %0 = load i32, i32* @patlen, align 4
  %sub = sub nsw i32 %0, 1
  %1 = load i32, i32* %stringlen.addr, align 4
  %sub1 = sub nsw i32 %sub, %1
  store i32 %sub1, i32* %i, align 4
  %2 = load i32, i32* %i, align 4
  %cmp = icmp sge i32 %2, 0
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i8* null, i8** %retval, align 8
  br label %return

if.end:                                           ; preds = %entry
  %3 = load i32, i32* %stringlen.addr, align 4
  %4 = load i8*, i8** %string.addr, align 8
  %idx.ext = sext i32 %3 to i64
  %add.ptr = getelementptr inbounds i8, i8* %4, i64 %idx.ext
  store i8* %add.ptr, i8** %string.addr, align 8
  br label %for.cond

for.cond:                                         ; preds = %if.end33, %if.end
  br label %while.cond

while.cond:                                       ; preds = %while.body, %for.cond
  %5 = load i8*, i8** %string.addr, align 8
  %6 = load i32, i32* %i, align 4
  %idxprom = sext i32 %6 to i64
  %arrayidx = getelementptr inbounds i8, i8* %5, i64 %idxprom
  %7 = load i8, i8* %arrayidx, align 1
  %idxprom2 = zext i8 %7 to i64
  %arrayidx3 = getelementptr inbounds [256 x i32], [256 x i32]* @skip, i64 0, i64 %idxprom2
  %8 = load i32, i32* %arrayidx3, align 4
  %9 = load i32, i32* %i, align 4
  %add = add nsw i32 %9, %8
  store i32 %add, i32* %i, align 4
  %cmp4 = icmp slt i32 %add, 0
  br i1 %cmp4, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  br label %while.cond, !llvm.loop !6

while.end:                                        ; preds = %while.cond
  %10 = load i32, i32* %i, align 4
  %11 = load i32, i32* %stringlen.addr, align 4
  %sub5 = sub nsw i32 32767, %11
  %cmp6 = icmp slt i32 %10, %sub5
  br i1 %cmp6, label %if.then7, label %if.end8

if.then7:                                         ; preds = %while.end
  store i8* null, i8** %retval, align 8
  br label %return

if.end8:                                          ; preds = %while.end
  %12 = load i32, i32* %i, align 4
  %sub9 = sub nsw i32 %12, 32767
  store i32 %sub9, i32* %i, align 4
  %13 = load i32, i32* @patlen, align 4
  %sub10 = sub nsw i32 %13, 1
  store i32 %sub10, i32* %j, align 4
  %14 = load i8*, i8** %string.addr, align 8
  %15 = load i32, i32* %i, align 4
  %16 = load i32, i32* %j, align 4
  %sub11 = sub nsw i32 %15, %16
  %idx.ext12 = sext i32 %sub11 to i64
  %add.ptr13 = getelementptr inbounds i8, i8* %14, i64 %idx.ext12
  store i8* %add.ptr13, i8** %s, align 8
  br label %while.cond14

while.cond14:                                     ; preds = %while.body23, %if.end8
  %17 = load i32, i32* %j, align 4
  %dec = add nsw i32 %17, -1
  store i32 %dec, i32* %j, align 4
  %cmp15 = icmp sge i32 %dec, 0
  br i1 %cmp15, label %land.rhs, label %land.end

land.rhs:                                         ; preds = %while.cond14
  %18 = load i8*, i8** %s, align 8
  %19 = load i32, i32* %j, align 4
  %idxprom16 = sext i32 %19 to i64
  %arrayidx17 = getelementptr inbounds i8, i8* %18, i64 %idxprom16
  %20 = load i8, i8* %arrayidx17, align 1
  %conv = sext i8 %20 to i32
  %21 = load i8*, i8** @pat, align 8
  %22 = load i32, i32* %j, align 4
  %idxprom18 = sext i32 %22 to i64
  %arrayidx19 = getelementptr inbounds i8, i8* %21, i64 %idxprom18
  %23 = load i8, i8* %arrayidx19, align 1
  %conv20 = zext i8 %23 to i32
  %cmp21 = icmp eq i32 %conv, %conv20
  br label %land.end

land.end:                                         ; preds = %land.rhs, %while.cond14
  %24 = phi i1 [ false, %while.cond14 ], [ %cmp21, %land.rhs ]
  br i1 %24, label %while.body23, label %while.end24

while.body23:                                     ; preds = %land.end
  br label %while.cond14, !llvm.loop !7

while.end24:                                      ; preds = %land.end
  %25 = load i32, i32* %j, align 4
  %cmp25 = icmp slt i32 %25, 0
  br i1 %cmp25, label %if.then27, label %if.end28

if.then27:                                        ; preds = %while.end24
  %26 = load i8*, i8** %s, align 8
  store i8* %26, i8** %retval, align 8
  br label %return

if.end28:                                         ; preds = %while.end24
  %27 = load i32, i32* @skip2, align 4
  %28 = load i32, i32* %i, align 4
  %add29 = add nsw i32 %28, %27
  store i32 %add29, i32* %i, align 4
  %cmp30 = icmp sge i32 %add29, 0
  br i1 %cmp30, label %if.then32, label %if.end33

if.then32:                                        ; preds = %if.end28
  store i8* null, i8** %retval, align 8
  br label %return

if.end33:                                         ; preds = %if.end28
  br label %for.cond

return:                                           ; preds = %if.then32, %if.then27, %if.then7, %if.then
  %29 = load i8*, i8** %retval, align 8
  ret i8* %29
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind readonly willreturn "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind readonly willreturn }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 13.0.0 (https://github.com/llvm/llvm-project.git f5ba3eea6746559513af7ed32db8083ad52661a3)"}
!2 = distinct !{!2, !3}
!3 = !{!"llvm.loop.mustprogress"}
!4 = distinct !{!4, !3}
!5 = distinct !{!5, !3}
!6 = distinct !{!6, !3}
!7 = distinct !{!7, !3}
