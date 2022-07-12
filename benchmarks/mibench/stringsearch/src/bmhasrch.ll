; ModuleID = 'bmhasrch.c'
source_filename = "bmhasrch.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@lowervec = dso_local global [256 x i8] c"\00\01\02\03\04\05\06\07\08\09\0A\0B\0C\0D\0E\0F\10\11\12\13\14\15\16\17\18\19\1A\1B\1C\1D\1E\1F !\22#$%&'()*+,-./0123456789:;<=>?@abcdefghijklmnopqrstuvwxyz[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\7Fcueaaaaceeeiiiaae\91\92ooouuyou\9B\9C\9D\9E\9Faiounn\A6\A7\A8\A9\AA\AB\AC\AD\AE\AF\B0\B1\B2\B3\B4\B5\B6\B7\B8\B9\BA\BB\BC\BD\BE\BF\C0\C1\C2\C3\C4\C5\C6\C7\C8\C9\CA\CB\CC\CD\CE\CF\D0\D1\D2\D3\D4\D5\D6\D7\D8\D9\DA\DB\DC\DD\DE\DF\E0\E1\E2\E3\E4\E5\E6\E7\E8\E9\EA\EB\EC\ED\EE\EF\F0\F1\F2\F3\F4\F5\F6\F7\F8\F9\FA\FB\FC\FD\FE\FF", align 16
@pat = internal global i8* null, align 8
@patlen = internal global i32 0, align 4
@skip = internal global [256 x i32] zeroinitializer, align 16
@skip2 = internal global i32 0, align 4

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @bmha_init(i8* %pattern) #0 {
entry:
  %pattern.addr = alloca i8*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i8* %pattern, i8** %pattern.addr, align 8
  %0 = load i8*, i8** %pattern.addr, align 8
  store i8* %0, i8** @pat, align 8
  %1 = load i8*, i8** %pattern.addr, align 8
  %call = call i64 @strlen(i8* %1) #2
  %conv = trunc i64 %call to i32
  store i32 %conv, i32* @patlen, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc32, %entry
  %2 = load i32, i32* %i, align 4
  %cmp = icmp sle i32 %2, 255
  br i1 %cmp, label %for.body, label %for.end33

for.body:                                         ; preds = %for.cond
  %3 = load i32, i32* @patlen, align 4
  %4 = load i32, i32* %i, align 4
  %idxprom = sext i32 %4 to i64
  %arrayidx = getelementptr inbounds [256 x i32], [256 x i32]* @skip, i64 0, i64 %idxprom
  store i32 %3, i32* %arrayidx, align 4
  %5 = load i32, i32* @patlen, align 4
  %sub = sub nsw i32 %5, 1
  store i32 %sub, i32* %j, align 4
  br label %for.cond2

for.cond2:                                        ; preds = %for.inc, %for.body
  %6 = load i32, i32* %j, align 4
  %cmp3 = icmp sge i32 %6, 0
  br i1 %cmp3, label %for.body5, label %for.end

for.body5:                                        ; preds = %for.cond2
  %7 = load i32, i32* %i, align 4
  %conv6 = trunc i32 %7 to i8
  %idxprom7 = zext i8 %conv6 to i64
  %arrayidx8 = getelementptr inbounds [256 x i8], [256 x i8]* @lowervec, i64 0, i64 %idxprom7
  %8 = load i8, i8* %arrayidx8, align 1
  %conv9 = zext i8 %8 to i32
  %9 = load i8*, i8** @pat, align 8
  %10 = load i32, i32* %j, align 4
  %idxprom10 = sext i32 %10 to i64
  %arrayidx11 = getelementptr inbounds i8, i8* %9, i64 %idxprom10
  %11 = load i8, i8* %arrayidx11, align 1
  %idxprom12 = zext i8 %11 to i64
  %arrayidx13 = getelementptr inbounds [256 x i8], [256 x i8]* @lowervec, i64 0, i64 %idxprom12
  %12 = load i8, i8* %arrayidx13, align 1
  %conv14 = zext i8 %12 to i32
  %cmp15 = icmp eq i32 %conv9, %conv14
  br i1 %cmp15, label %if.then, label %if.end

if.then:                                          ; preds = %for.body5
  br label %for.end

if.end:                                           ; preds = %for.body5
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %13 = load i32, i32* %j, align 4
  %dec = add nsw i32 %13, -1
  store i32 %dec, i32* %j, align 4
  br label %for.cond2, !llvm.loop !2

for.end:                                          ; preds = %if.then, %for.cond2
  %14 = load i32, i32* %j, align 4
  %cmp17 = icmp sge i32 %14, 0
  br i1 %cmp17, label %if.then19, label %if.end24

if.then19:                                        ; preds = %for.end
  %15 = load i32, i32* @patlen, align 4
  %16 = load i32, i32* %j, align 4
  %sub20 = sub nsw i32 %15, %16
  %sub21 = sub nsw i32 %sub20, 1
  %17 = load i32, i32* %i, align 4
  %idxprom22 = sext i32 %17 to i64
  %arrayidx23 = getelementptr inbounds [256 x i32], [256 x i32]* @skip, i64 0, i64 %idxprom22
  store i32 %sub21, i32* %arrayidx23, align 4
  br label %if.end24

if.end24:                                         ; preds = %if.then19, %for.end
  %18 = load i32, i32* %j, align 4
  %19 = load i32, i32* @patlen, align 4
  %sub25 = sub nsw i32 %19, 1
  %cmp26 = icmp eq i32 %18, %sub25
  br i1 %cmp26, label %if.then28, label %if.end31

if.then28:                                        ; preds = %if.end24
  %20 = load i32, i32* %i, align 4
  %idxprom29 = sext i32 %20 to i64
  %arrayidx30 = getelementptr inbounds [256 x i32], [256 x i32]* @skip, i64 0, i64 %idxprom29
  store i32 32767, i32* %arrayidx30, align 4
  br label %if.end31

if.end31:                                         ; preds = %if.then28, %if.end24
  br label %for.inc32

for.inc32:                                        ; preds = %if.end31
  %21 = load i32, i32* %i, align 4
  %inc = add nsw i32 %21, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond, !llvm.loop !4

for.end33:                                        ; preds = %for.cond
  %22 = load i32, i32* @patlen, align 4
  store i32 %22, i32* @skip2, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond34

for.cond34:                                       ; preds = %for.inc56, %for.end33
  %23 = load i32, i32* %i, align 4
  %24 = load i32, i32* @patlen, align 4
  %sub35 = sub nsw i32 %24, 1
  %cmp36 = icmp slt i32 %23, %sub35
  br i1 %cmp36, label %for.body38, label %for.end58

for.body38:                                       ; preds = %for.cond34
  %25 = load i8*, i8** @pat, align 8
  %26 = load i32, i32* %i, align 4
  %idxprom39 = sext i32 %26 to i64
  %arrayidx40 = getelementptr inbounds i8, i8* %25, i64 %idxprom39
  %27 = load i8, i8* %arrayidx40, align 1
  %idxprom41 = zext i8 %27 to i64
  %arrayidx42 = getelementptr inbounds [256 x i8], [256 x i8]* @lowervec, i64 0, i64 %idxprom41
  %28 = load i8, i8* %arrayidx42, align 1
  %conv43 = zext i8 %28 to i32
  %29 = load i8*, i8** @pat, align 8
  %30 = load i32, i32* @patlen, align 4
  %sub44 = sub nsw i32 %30, 1
  %idxprom45 = sext i32 %sub44 to i64
  %arrayidx46 = getelementptr inbounds i8, i8* %29, i64 %idxprom45
  %31 = load i8, i8* %arrayidx46, align 1
  %idxprom47 = zext i8 %31 to i64
  %arrayidx48 = getelementptr inbounds [256 x i8], [256 x i8]* @lowervec, i64 0, i64 %idxprom47
  %32 = load i8, i8* %arrayidx48, align 1
  %conv49 = zext i8 %32 to i32
  %cmp50 = icmp eq i32 %conv43, %conv49
  br i1 %cmp50, label %if.then52, label %if.end55

if.then52:                                        ; preds = %for.body38
  %33 = load i32, i32* @patlen, align 4
  %34 = load i32, i32* %i, align 4
  %sub53 = sub nsw i32 %33, %34
  %sub54 = sub nsw i32 %sub53, 1
  store i32 %sub54, i32* @skip2, align 4
  br label %if.end55

if.end55:                                         ; preds = %if.then52, %for.body38
  br label %for.inc56

for.inc56:                                        ; preds = %if.end55
  %35 = load i32, i32* %i, align 4
  %inc57 = add nsw i32 %35, 1
  store i32 %inc57, i32* %i, align 4
  br label %for.cond34, !llvm.loop !5

for.end58:                                        ; preds = %for.cond34
  ret void
}

; Function Attrs: nounwind readonly willreturn
declare dso_local i64 @strlen(i8*) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8* @bmha_search(i8* %string, i32 %stringlen) #0 {
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

for.cond:                                         ; preds = %if.end37, %if.end
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

while.cond14:                                     ; preds = %while.body27, %if.end8
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
  %idxprom18 = zext i8 %20 to i64
  %arrayidx19 = getelementptr inbounds [256 x i8], [256 x i8]* @lowervec, i64 0, i64 %idxprom18
  %21 = load i8, i8* %arrayidx19, align 1
  %conv = zext i8 %21 to i32
  %22 = load i8*, i8** @pat, align 8
  %23 = load i32, i32* %j, align 4
  %idxprom20 = sext i32 %23 to i64
  %arrayidx21 = getelementptr inbounds i8, i8* %22, i64 %idxprom20
  %24 = load i8, i8* %arrayidx21, align 1
  %idxprom22 = zext i8 %24 to i64
  %arrayidx23 = getelementptr inbounds [256 x i8], [256 x i8]* @lowervec, i64 0, i64 %idxprom22
  %25 = load i8, i8* %arrayidx23, align 1
  %conv24 = zext i8 %25 to i32
  %cmp25 = icmp eq i32 %conv, %conv24
  br label %land.end

land.end:                                         ; preds = %land.rhs, %while.cond14
  %26 = phi i1 [ false, %while.cond14 ], [ %cmp25, %land.rhs ]
  br i1 %26, label %while.body27, label %while.end28

while.body27:                                     ; preds = %land.end
  br label %while.cond14, !llvm.loop !7

while.end28:                                      ; preds = %land.end
  %27 = load i32, i32* %j, align 4
  %cmp29 = icmp slt i32 %27, 0
  br i1 %cmp29, label %if.then31, label %if.end32

if.then31:                                        ; preds = %while.end28
  %28 = load i8*, i8** %s, align 8
  store i8* %28, i8** %retval, align 8
  br label %return

if.end32:                                         ; preds = %while.end28
  %29 = load i32, i32* @skip2, align 4
  %30 = load i32, i32* %i, align 4
  %add33 = add nsw i32 %30, %29
  store i32 %add33, i32* %i, align 4
  %cmp34 = icmp sge i32 %add33, 0
  br i1 %cmp34, label %if.then36, label %if.end37

if.then36:                                        ; preds = %if.end32
  store i8* null, i8** %retval, align 8
  br label %return

if.end37:                                         ; preds = %if.end32
  br label %for.cond

return:                                           ; preds = %if.then36, %if.then31, %if.then7, %if.then
  %31 = load i8*, i8** %retval, align 8
  ret i8* %31
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
