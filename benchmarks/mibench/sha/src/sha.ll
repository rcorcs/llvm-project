; ModuleID = 'sha.c'
source_filename = "sha.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.SHA_INFO = type { [5 x i64], i64, i64, [16 x i64] }
%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }

@.str = private unnamed_addr constant [31 x i8] c"%08lx %08lx %08lx %08lx %08lx\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @sha_init(%struct.SHA_INFO* %sha_info) #0 {
entry:
  %sha_info.addr = alloca %struct.SHA_INFO*, align 8
  store %struct.SHA_INFO* %sha_info, %struct.SHA_INFO** %sha_info.addr, align 8
  %0 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %0, i32 0, i32 0
  %arrayidx = getelementptr inbounds [5 x i64], [5 x i64]* %digest, i64 0, i64 0
  store i64 1732584193, i64* %arrayidx, align 8
  %1 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest1 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %1, i32 0, i32 0
  %arrayidx2 = getelementptr inbounds [5 x i64], [5 x i64]* %digest1, i64 0, i64 1
  store i64 4023233417, i64* %arrayidx2, align 8
  %2 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest3 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %2, i32 0, i32 0
  %arrayidx4 = getelementptr inbounds [5 x i64], [5 x i64]* %digest3, i64 0, i64 2
  store i64 2562383102, i64* %arrayidx4, align 8
  %3 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest5 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %3, i32 0, i32 0
  %arrayidx6 = getelementptr inbounds [5 x i64], [5 x i64]* %digest5, i64 0, i64 3
  store i64 271733878, i64* %arrayidx6, align 8
  %4 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest7 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %4, i32 0, i32 0
  %arrayidx8 = getelementptr inbounds [5 x i64], [5 x i64]* %digest7, i64 0, i64 4
  store i64 3285377520, i64* %arrayidx8, align 8
  %5 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %count_lo = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %5, i32 0, i32 1
  store i64 0, i64* %count_lo, align 8
  %6 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %count_hi = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %6, i32 0, i32 2
  store i64 0, i64* %count_hi, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @sha_update(%struct.SHA_INFO* %sha_info, i8* %buffer, i32 %count) #0 {
entry:
  %sha_info.addr = alloca %struct.SHA_INFO*, align 8
  %buffer.addr = alloca i8*, align 8
  %count.addr = alloca i32, align 4
  store %struct.SHA_INFO* %sha_info, %struct.SHA_INFO** %sha_info.addr, align 8
  store i8* %buffer, i8** %buffer.addr, align 8
  store i32 %count, i32* %count.addr, align 4
  %0 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %count_lo = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %0, i32 0, i32 1
  %1 = load i64, i64* %count_lo, align 8
  %2 = load i32, i32* %count.addr, align 4
  %conv = sext i32 %2 to i64
  %shl = shl i64 %conv, 3
  %add = add i64 %1, %shl
  %3 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %count_lo1 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %3, i32 0, i32 1
  %4 = load i64, i64* %count_lo1, align 8
  %cmp = icmp ult i64 %add, %4
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %5 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %count_hi = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %5, i32 0, i32 2
  %6 = load i64, i64* %count_hi, align 8
  %inc = add i64 %6, 1
  store i64 %inc, i64* %count_hi, align 8
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %7 = load i32, i32* %count.addr, align 4
  %conv3 = sext i32 %7 to i64
  %shl4 = shl i64 %conv3, 3
  %8 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %count_lo5 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %8, i32 0, i32 1
  %9 = load i64, i64* %count_lo5, align 8
  %add6 = add i64 %9, %shl4
  store i64 %add6, i64* %count_lo5, align 8
  %10 = load i32, i32* %count.addr, align 4
  %conv7 = sext i32 %10 to i64
  %shr = lshr i64 %conv7, 29
  %11 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %count_hi8 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %11, i32 0, i32 2
  %12 = load i64, i64* %count_hi8, align 8
  %add9 = add i64 %12, %shr
  store i64 %add9, i64* %count_hi8, align 8
  br label %while.cond

while.cond:                                       ; preds = %while.body, %if.end
  %13 = load i32, i32* %count.addr, align 4
  %cmp10 = icmp sge i32 %13, 64
  br i1 %cmp10, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %14 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %data = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %14, i32 0, i32 3
  %arraydecay = getelementptr inbounds [16 x i64], [16 x i64]* %data, i64 0, i64 0
  %15 = bitcast i64* %arraydecay to i8*
  %16 = load i8*, i8** %buffer.addr, align 8
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %15, i8* align 1 %16, i64 64, i1 false)
  %17 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %data12 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %17, i32 0, i32 3
  %arraydecay13 = getelementptr inbounds [16 x i64], [16 x i64]* %data12, i64 0, i64 0
  call void @byte_reverse(i64* %arraydecay13, i32 64)
  %18 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  call void @sha_transform(%struct.SHA_INFO* %18)
  %19 = load i8*, i8** %buffer.addr, align 8
  %add.ptr = getelementptr inbounds i8, i8* %19, i64 64
  store i8* %add.ptr, i8** %buffer.addr, align 8
  %20 = load i32, i32* %count.addr, align 4
  %sub = sub nsw i32 %20, 64
  store i32 %sub, i32* %count.addr, align 4
  br label %while.cond, !llvm.loop !2

while.end:                                        ; preds = %while.cond
  %21 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %data14 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %21, i32 0, i32 3
  %arraydecay15 = getelementptr inbounds [16 x i64], [16 x i64]* %data14, i64 0, i64 0
  %22 = bitcast i64* %arraydecay15 to i8*
  %23 = load i8*, i8** %buffer.addr, align 8
  %24 = load i32, i32* %count.addr, align 4
  %conv16 = sext i32 %24 to i64
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %22, i8* align 1 %23, i64 %conv16, i1 false)
  ret void
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #1

; Function Attrs: noinline nounwind optnone uwtable
define internal void @byte_reverse(i64* %buffer, i32 %count) #0 {
entry:
  %buffer.addr = alloca i64*, align 8
  %count.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %ct = alloca [4 x i8], align 1
  %cp = alloca i8*, align 8
  store i64* %buffer, i64** %buffer.addr, align 8
  store i32 %count, i32* %count.addr, align 4
  %0 = load i32, i32* %count.addr, align 4
  %conv = sext i32 %0 to i64
  %div = udiv i64 %conv, 8
  %conv1 = trunc i64 %div to i32
  store i32 %conv1, i32* %count.addr, align 4
  %1 = load i64*, i64** %buffer.addr, align 8
  %2 = bitcast i64* %1 to i8*
  store i8* %2, i8** %cp, align 8
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %3 = load i32, i32* %i, align 4
  %4 = load i32, i32* %count.addr, align 4
  %cmp = icmp slt i32 %3, %4
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %5 = load i8*, i8** %cp, align 8
  %arrayidx = getelementptr inbounds i8, i8* %5, i64 0
  %6 = load i8, i8* %arrayidx, align 1
  %arrayidx3 = getelementptr inbounds [4 x i8], [4 x i8]* %ct, i64 0, i64 0
  store i8 %6, i8* %arrayidx3, align 1
  %7 = load i8*, i8** %cp, align 8
  %arrayidx4 = getelementptr inbounds i8, i8* %7, i64 1
  %8 = load i8, i8* %arrayidx4, align 1
  %arrayidx5 = getelementptr inbounds [4 x i8], [4 x i8]* %ct, i64 0, i64 1
  store i8 %8, i8* %arrayidx5, align 1
  %9 = load i8*, i8** %cp, align 8
  %arrayidx6 = getelementptr inbounds i8, i8* %9, i64 2
  %10 = load i8, i8* %arrayidx6, align 1
  %arrayidx7 = getelementptr inbounds [4 x i8], [4 x i8]* %ct, i64 0, i64 2
  store i8 %10, i8* %arrayidx7, align 1
  %11 = load i8*, i8** %cp, align 8
  %arrayidx8 = getelementptr inbounds i8, i8* %11, i64 3
  %12 = load i8, i8* %arrayidx8, align 1
  %arrayidx9 = getelementptr inbounds [4 x i8], [4 x i8]* %ct, i64 0, i64 3
  store i8 %12, i8* %arrayidx9, align 1
  %arrayidx10 = getelementptr inbounds [4 x i8], [4 x i8]* %ct, i64 0, i64 3
  %13 = load i8, i8* %arrayidx10, align 1
  %14 = load i8*, i8** %cp, align 8
  %arrayidx11 = getelementptr inbounds i8, i8* %14, i64 0
  store i8 %13, i8* %arrayidx11, align 1
  %arrayidx12 = getelementptr inbounds [4 x i8], [4 x i8]* %ct, i64 0, i64 2
  %15 = load i8, i8* %arrayidx12, align 1
  %16 = load i8*, i8** %cp, align 8
  %arrayidx13 = getelementptr inbounds i8, i8* %16, i64 1
  store i8 %15, i8* %arrayidx13, align 1
  %arrayidx14 = getelementptr inbounds [4 x i8], [4 x i8]* %ct, i64 0, i64 1
  %17 = load i8, i8* %arrayidx14, align 1
  %18 = load i8*, i8** %cp, align 8
  %arrayidx15 = getelementptr inbounds i8, i8* %18, i64 2
  store i8 %17, i8* %arrayidx15, align 1
  %arrayidx16 = getelementptr inbounds [4 x i8], [4 x i8]* %ct, i64 0, i64 0
  %19 = load i8, i8* %arrayidx16, align 1
  %20 = load i8*, i8** %cp, align 8
  %arrayidx17 = getelementptr inbounds i8, i8* %20, i64 3
  store i8 %19, i8* %arrayidx17, align 1
  %21 = load i8*, i8** %cp, align 8
  %add.ptr = getelementptr inbounds i8, i8* %21, i64 8
  store i8* %add.ptr, i8** %cp, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %22 = load i32, i32* %i, align 4
  %inc = add nsw i32 %22, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond, !llvm.loop !4

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @sha_transform(%struct.SHA_INFO* %sha_info) #0 {
entry:
  %sha_info.addr = alloca %struct.SHA_INFO*, align 8
  %i = alloca i32, align 4
  %temp = alloca i64, align 8
  %A = alloca i64, align 8
  %B = alloca i64, align 8
  %C = alloca i64, align 8
  %D = alloca i64, align 8
  %E = alloca i64, align 8
  %W = alloca [80 x i64], align 16
  store %struct.SHA_INFO* %sha_info, %struct.SHA_INFO** %sha_info.addr, align 8
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 16
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %1 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %data = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %1, i32 0, i32 3
  %2 = load i32, i32* %i, align 4
  %idxprom = sext i32 %2 to i64
  %arrayidx = getelementptr inbounds [16 x i64], [16 x i64]* %data, i64 0, i64 %idxprom
  %3 = load i64, i64* %arrayidx, align 8
  %4 = load i32, i32* %i, align 4
  %idxprom1 = sext i32 %4 to i64
  %arrayidx2 = getelementptr inbounds [80 x i64], [80 x i64]* %W, i64 0, i64 %idxprom1
  store i64 %3, i64* %arrayidx2, align 8
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %5 = load i32, i32* %i, align 4
  %inc = add nsw i32 %5, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond, !llvm.loop !5

for.end:                                          ; preds = %for.cond
  store i32 16, i32* %i, align 4
  br label %for.cond3

for.cond3:                                        ; preds = %for.inc21, %for.end
  %6 = load i32, i32* %i, align 4
  %cmp4 = icmp slt i32 %6, 80
  br i1 %cmp4, label %for.body5, label %for.end23

for.body5:                                        ; preds = %for.cond3
  %7 = load i32, i32* %i, align 4
  %sub = sub nsw i32 %7, 3
  %idxprom6 = sext i32 %sub to i64
  %arrayidx7 = getelementptr inbounds [80 x i64], [80 x i64]* %W, i64 0, i64 %idxprom6
  %8 = load i64, i64* %arrayidx7, align 8
  %9 = load i32, i32* %i, align 4
  %sub8 = sub nsw i32 %9, 8
  %idxprom9 = sext i32 %sub8 to i64
  %arrayidx10 = getelementptr inbounds [80 x i64], [80 x i64]* %W, i64 0, i64 %idxprom9
  %10 = load i64, i64* %arrayidx10, align 8
  %xor = xor i64 %8, %10
  %11 = load i32, i32* %i, align 4
  %sub11 = sub nsw i32 %11, 14
  %idxprom12 = sext i32 %sub11 to i64
  %arrayidx13 = getelementptr inbounds [80 x i64], [80 x i64]* %W, i64 0, i64 %idxprom12
  %12 = load i64, i64* %arrayidx13, align 8
  %xor14 = xor i64 %xor, %12
  %13 = load i32, i32* %i, align 4
  %sub15 = sub nsw i32 %13, 16
  %idxprom16 = sext i32 %sub15 to i64
  %arrayidx17 = getelementptr inbounds [80 x i64], [80 x i64]* %W, i64 0, i64 %idxprom16
  %14 = load i64, i64* %arrayidx17, align 8
  %xor18 = xor i64 %xor14, %14
  %15 = load i32, i32* %i, align 4
  %idxprom19 = sext i32 %15 to i64
  %arrayidx20 = getelementptr inbounds [80 x i64], [80 x i64]* %W, i64 0, i64 %idxprom19
  store i64 %xor18, i64* %arrayidx20, align 8
  br label %for.inc21

for.inc21:                                        ; preds = %for.body5
  %16 = load i32, i32* %i, align 4
  %inc22 = add nsw i32 %16, 1
  store i32 %inc22, i32* %i, align 4
  br label %for.cond3, !llvm.loop !6

for.end23:                                        ; preds = %for.cond3
  %17 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %17, i32 0, i32 0
  %arrayidx24 = getelementptr inbounds [5 x i64], [5 x i64]* %digest, i64 0, i64 0
  %18 = load i64, i64* %arrayidx24, align 8
  store i64 %18, i64* %A, align 8
  %19 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest25 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %19, i32 0, i32 0
  %arrayidx26 = getelementptr inbounds [5 x i64], [5 x i64]* %digest25, i64 0, i64 1
  %20 = load i64, i64* %arrayidx26, align 8
  store i64 %20, i64* %B, align 8
  %21 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest27 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %21, i32 0, i32 0
  %arrayidx28 = getelementptr inbounds [5 x i64], [5 x i64]* %digest27, i64 0, i64 2
  %22 = load i64, i64* %arrayidx28, align 8
  store i64 %22, i64* %C, align 8
  %23 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest29 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %23, i32 0, i32 0
  %arrayidx30 = getelementptr inbounds [5 x i64], [5 x i64]* %digest29, i64 0, i64 3
  %24 = load i64, i64* %arrayidx30, align 8
  store i64 %24, i64* %D, align 8
  %25 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest31 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %25, i32 0, i32 0
  %arrayidx32 = getelementptr inbounds [5 x i64], [5 x i64]* %digest31, i64 0, i64 4
  %26 = load i64, i64* %arrayidx32, align 8
  store i64 %26, i64* %E, align 8
  store i32 0, i32* %i, align 4
  br label %for.cond33

for.cond33:                                       ; preds = %for.inc46, %for.end23
  %27 = load i32, i32* %i, align 4
  %cmp34 = icmp slt i32 %27, 20
  br i1 %cmp34, label %for.body35, label %for.end48

for.body35:                                       ; preds = %for.cond33
  %28 = load i64, i64* %A, align 8
  %shl = shl i64 %28, 5
  %29 = load i64, i64* %A, align 8
  %shr = lshr i64 %29, 27
  %or = or i64 %shl, %shr
  %30 = load i64, i64* %B, align 8
  %31 = load i64, i64* %C, align 8
  %and = and i64 %30, %31
  %32 = load i64, i64* %B, align 8
  %neg = xor i64 %32, -1
  %33 = load i64, i64* %D, align 8
  %and36 = and i64 %neg, %33
  %or37 = or i64 %and, %and36
  %add = add i64 %or, %or37
  %34 = load i64, i64* %E, align 8
  %add38 = add i64 %add, %34
  %35 = load i32, i32* %i, align 4
  %idxprom39 = sext i32 %35 to i64
  %arrayidx40 = getelementptr inbounds [80 x i64], [80 x i64]* %W, i64 0, i64 %idxprom39
  %36 = load i64, i64* %arrayidx40, align 8
  %add41 = add i64 %add38, %36
  %add42 = add i64 %add41, 1518500249
  store i64 %add42, i64* %temp, align 8
  %37 = load i64, i64* %D, align 8
  store i64 %37, i64* %E, align 8
  %38 = load i64, i64* %C, align 8
  store i64 %38, i64* %D, align 8
  %39 = load i64, i64* %B, align 8
  %shl43 = shl i64 %39, 30
  %40 = load i64, i64* %B, align 8
  %shr44 = lshr i64 %40, 2
  %or45 = or i64 %shl43, %shr44
  store i64 %or45, i64* %C, align 8
  %41 = load i64, i64* %A, align 8
  store i64 %41, i64* %B, align 8
  %42 = load i64, i64* %temp, align 8
  store i64 %42, i64* %A, align 8
  br label %for.inc46

for.inc46:                                        ; preds = %for.body35
  %43 = load i32, i32* %i, align 4
  %inc47 = add nsw i32 %43, 1
  store i32 %inc47, i32* %i, align 4
  br label %for.cond33, !llvm.loop !7

for.end48:                                        ; preds = %for.cond33
  store i32 20, i32* %i, align 4
  br label %for.cond49

for.cond49:                                       ; preds = %for.inc66, %for.end48
  %44 = load i32, i32* %i, align 4
  %cmp50 = icmp slt i32 %44, 40
  br i1 %cmp50, label %for.body51, label %for.end68

for.body51:                                       ; preds = %for.cond49
  %45 = load i64, i64* %A, align 8
  %shl52 = shl i64 %45, 5
  %46 = load i64, i64* %A, align 8
  %shr53 = lshr i64 %46, 27
  %or54 = or i64 %shl52, %shr53
  %47 = load i64, i64* %B, align 8
  %48 = load i64, i64* %C, align 8
  %xor55 = xor i64 %47, %48
  %49 = load i64, i64* %D, align 8
  %xor56 = xor i64 %xor55, %49
  %add57 = add i64 %or54, %xor56
  %50 = load i64, i64* %E, align 8
  %add58 = add i64 %add57, %50
  %51 = load i32, i32* %i, align 4
  %idxprom59 = sext i32 %51 to i64
  %arrayidx60 = getelementptr inbounds [80 x i64], [80 x i64]* %W, i64 0, i64 %idxprom59
  %52 = load i64, i64* %arrayidx60, align 8
  %add61 = add i64 %add58, %52
  %add62 = add i64 %add61, 1859775393
  store i64 %add62, i64* %temp, align 8
  %53 = load i64, i64* %D, align 8
  store i64 %53, i64* %E, align 8
  %54 = load i64, i64* %C, align 8
  store i64 %54, i64* %D, align 8
  %55 = load i64, i64* %B, align 8
  %shl63 = shl i64 %55, 30
  %56 = load i64, i64* %B, align 8
  %shr64 = lshr i64 %56, 2
  %or65 = or i64 %shl63, %shr64
  store i64 %or65, i64* %C, align 8
  %57 = load i64, i64* %A, align 8
  store i64 %57, i64* %B, align 8
  %58 = load i64, i64* %temp, align 8
  store i64 %58, i64* %A, align 8
  br label %for.inc66

for.inc66:                                        ; preds = %for.body51
  %59 = load i32, i32* %i, align 4
  %inc67 = add nsw i32 %59, 1
  store i32 %inc67, i32* %i, align 4
  br label %for.cond49, !llvm.loop !8

for.end68:                                        ; preds = %for.cond49
  store i32 40, i32* %i, align 4
  br label %for.cond69

for.cond69:                                       ; preds = %for.inc89, %for.end68
  %60 = load i32, i32* %i, align 4
  %cmp70 = icmp slt i32 %60, 60
  br i1 %cmp70, label %for.body71, label %for.end91

for.body71:                                       ; preds = %for.cond69
  %61 = load i64, i64* %A, align 8
  %shl72 = shl i64 %61, 5
  %62 = load i64, i64* %A, align 8
  %shr73 = lshr i64 %62, 27
  %or74 = or i64 %shl72, %shr73
  %63 = load i64, i64* %B, align 8
  %64 = load i64, i64* %C, align 8
  %and75 = and i64 %63, %64
  %65 = load i64, i64* %B, align 8
  %66 = load i64, i64* %D, align 8
  %and76 = and i64 %65, %66
  %or77 = or i64 %and75, %and76
  %67 = load i64, i64* %C, align 8
  %68 = load i64, i64* %D, align 8
  %and78 = and i64 %67, %68
  %or79 = or i64 %or77, %and78
  %add80 = add i64 %or74, %or79
  %69 = load i64, i64* %E, align 8
  %add81 = add i64 %add80, %69
  %70 = load i32, i32* %i, align 4
  %idxprom82 = sext i32 %70 to i64
  %arrayidx83 = getelementptr inbounds [80 x i64], [80 x i64]* %W, i64 0, i64 %idxprom82
  %71 = load i64, i64* %arrayidx83, align 8
  %add84 = add i64 %add81, %71
  %add85 = add i64 %add84, 2400959708
  store i64 %add85, i64* %temp, align 8
  %72 = load i64, i64* %D, align 8
  store i64 %72, i64* %E, align 8
  %73 = load i64, i64* %C, align 8
  store i64 %73, i64* %D, align 8
  %74 = load i64, i64* %B, align 8
  %shl86 = shl i64 %74, 30
  %75 = load i64, i64* %B, align 8
  %shr87 = lshr i64 %75, 2
  %or88 = or i64 %shl86, %shr87
  store i64 %or88, i64* %C, align 8
  %76 = load i64, i64* %A, align 8
  store i64 %76, i64* %B, align 8
  %77 = load i64, i64* %temp, align 8
  store i64 %77, i64* %A, align 8
  br label %for.inc89

for.inc89:                                        ; preds = %for.body71
  %78 = load i32, i32* %i, align 4
  %inc90 = add nsw i32 %78, 1
  store i32 %inc90, i32* %i, align 4
  br label %for.cond69, !llvm.loop !9

for.end91:                                        ; preds = %for.cond69
  store i32 60, i32* %i, align 4
  br label %for.cond92

for.cond92:                                       ; preds = %for.inc109, %for.end91
  %79 = load i32, i32* %i, align 4
  %cmp93 = icmp slt i32 %79, 80
  br i1 %cmp93, label %for.body94, label %for.end111

for.body94:                                       ; preds = %for.cond92
  %80 = load i64, i64* %A, align 8
  %shl95 = shl i64 %80, 5
  %81 = load i64, i64* %A, align 8
  %shr96 = lshr i64 %81, 27
  %or97 = or i64 %shl95, %shr96
  %82 = load i64, i64* %B, align 8
  %83 = load i64, i64* %C, align 8
  %xor98 = xor i64 %82, %83
  %84 = load i64, i64* %D, align 8
  %xor99 = xor i64 %xor98, %84
  %add100 = add i64 %or97, %xor99
  %85 = load i64, i64* %E, align 8
  %add101 = add i64 %add100, %85
  %86 = load i32, i32* %i, align 4
  %idxprom102 = sext i32 %86 to i64
  %arrayidx103 = getelementptr inbounds [80 x i64], [80 x i64]* %W, i64 0, i64 %idxprom102
  %87 = load i64, i64* %arrayidx103, align 8
  %add104 = add i64 %add101, %87
  %add105 = add i64 %add104, 3395469782
  store i64 %add105, i64* %temp, align 8
  %88 = load i64, i64* %D, align 8
  store i64 %88, i64* %E, align 8
  %89 = load i64, i64* %C, align 8
  store i64 %89, i64* %D, align 8
  %90 = load i64, i64* %B, align 8
  %shl106 = shl i64 %90, 30
  %91 = load i64, i64* %B, align 8
  %shr107 = lshr i64 %91, 2
  %or108 = or i64 %shl106, %shr107
  store i64 %or108, i64* %C, align 8
  %92 = load i64, i64* %A, align 8
  store i64 %92, i64* %B, align 8
  %93 = load i64, i64* %temp, align 8
  store i64 %93, i64* %A, align 8
  br label %for.inc109

for.inc109:                                       ; preds = %for.body94
  %94 = load i32, i32* %i, align 4
  %inc110 = add nsw i32 %94, 1
  store i32 %inc110, i32* %i, align 4
  br label %for.cond92, !llvm.loop !10

for.end111:                                       ; preds = %for.cond92
  %95 = load i64, i64* %A, align 8
  %96 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest112 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %96, i32 0, i32 0
  %arrayidx113 = getelementptr inbounds [5 x i64], [5 x i64]* %digest112, i64 0, i64 0
  %97 = load i64, i64* %arrayidx113, align 8
  %add114 = add i64 %97, %95
  store i64 %add114, i64* %arrayidx113, align 8
  %98 = load i64, i64* %B, align 8
  %99 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest115 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %99, i32 0, i32 0
  %arrayidx116 = getelementptr inbounds [5 x i64], [5 x i64]* %digest115, i64 0, i64 1
  %100 = load i64, i64* %arrayidx116, align 8
  %add117 = add i64 %100, %98
  store i64 %add117, i64* %arrayidx116, align 8
  %101 = load i64, i64* %C, align 8
  %102 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest118 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %102, i32 0, i32 0
  %arrayidx119 = getelementptr inbounds [5 x i64], [5 x i64]* %digest118, i64 0, i64 2
  %103 = load i64, i64* %arrayidx119, align 8
  %add120 = add i64 %103, %101
  store i64 %add120, i64* %arrayidx119, align 8
  %104 = load i64, i64* %D, align 8
  %105 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest121 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %105, i32 0, i32 0
  %arrayidx122 = getelementptr inbounds [5 x i64], [5 x i64]* %digest121, i64 0, i64 3
  %106 = load i64, i64* %arrayidx122, align 8
  %add123 = add i64 %106, %104
  store i64 %add123, i64* %arrayidx122, align 8
  %107 = load i64, i64* %E, align 8
  %108 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest124 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %108, i32 0, i32 0
  %arrayidx125 = getelementptr inbounds [5 x i64], [5 x i64]* %digest124, i64 0, i64 4
  %109 = load i64, i64* %arrayidx125, align 8
  %add126 = add i64 %109, %107
  store i64 %add126, i64* %arrayidx125, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @sha_final(%struct.SHA_INFO* %sha_info) #0 {
entry:
  %sha_info.addr = alloca %struct.SHA_INFO*, align 8
  %count = alloca i32, align 4
  %lo_bit_count = alloca i64, align 8
  %hi_bit_count = alloca i64, align 8
  store %struct.SHA_INFO* %sha_info, %struct.SHA_INFO** %sha_info.addr, align 8
  %0 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %count_lo = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %0, i32 0, i32 1
  %1 = load i64, i64* %count_lo, align 8
  store i64 %1, i64* %lo_bit_count, align 8
  %2 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %count_hi = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %2, i32 0, i32 2
  %3 = load i64, i64* %count_hi, align 8
  store i64 %3, i64* %hi_bit_count, align 8
  %4 = load i64, i64* %lo_bit_count, align 8
  %shr = lshr i64 %4, 3
  %and = and i64 %shr, 63
  %conv = trunc i64 %and to i32
  store i32 %conv, i32* %count, align 4
  %5 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %data = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %5, i32 0, i32 3
  %arraydecay = getelementptr inbounds [16 x i64], [16 x i64]* %data, i64 0, i64 0
  %6 = bitcast i64* %arraydecay to i8*
  %7 = load i32, i32* %count, align 4
  %inc = add nsw i32 %7, 1
  store i32 %inc, i32* %count, align 4
  %idxprom = sext i32 %7 to i64
  %arrayidx = getelementptr inbounds i8, i8* %6, i64 %idxprom
  store i8 -128, i8* %arrayidx, align 1
  %8 = load i32, i32* %count, align 4
  %cmp = icmp sgt i32 %8, 56
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %9 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %data2 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %9, i32 0, i32 3
  %10 = bitcast [16 x i64]* %data2 to i8*
  %11 = load i32, i32* %count, align 4
  %idx.ext = sext i32 %11 to i64
  %add.ptr = getelementptr inbounds i8, i8* %10, i64 %idx.ext
  %12 = load i32, i32* %count, align 4
  %sub = sub nsw i32 64, %12
  %conv3 = sext i32 %sub to i64
  call void @llvm.memset.p0i8.i64(i8* align 1 %add.ptr, i8 0, i64 %conv3, i1 false)
  %13 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %data4 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %13, i32 0, i32 3
  %arraydecay5 = getelementptr inbounds [16 x i64], [16 x i64]* %data4, i64 0, i64 0
  call void @byte_reverse(i64* %arraydecay5, i32 64)
  %14 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  call void @sha_transform(%struct.SHA_INFO* %14)
  %15 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %data6 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %15, i32 0, i32 3
  %16 = bitcast [16 x i64]* %data6 to i8*
  call void @llvm.memset.p0i8.i64(i8* align 8 %16, i8 0, i64 56, i1 false)
  br label %if.end

if.else:                                          ; preds = %entry
  %17 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %data7 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %17, i32 0, i32 3
  %18 = bitcast [16 x i64]* %data7 to i8*
  %19 = load i32, i32* %count, align 4
  %idx.ext8 = sext i32 %19 to i64
  %add.ptr9 = getelementptr inbounds i8, i8* %18, i64 %idx.ext8
  %20 = load i32, i32* %count, align 4
  %sub10 = sub nsw i32 56, %20
  %conv11 = sext i32 %sub10 to i64
  call void @llvm.memset.p0i8.i64(i8* align 1 %add.ptr9, i8 0, i64 %conv11, i1 false)
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %21 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %data12 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %21, i32 0, i32 3
  %arraydecay13 = getelementptr inbounds [16 x i64], [16 x i64]* %data12, i64 0, i64 0
  call void @byte_reverse(i64* %arraydecay13, i32 64)
  %22 = load i64, i64* %hi_bit_count, align 8
  %23 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %data14 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %23, i32 0, i32 3
  %arrayidx15 = getelementptr inbounds [16 x i64], [16 x i64]* %data14, i64 0, i64 14
  store i64 %22, i64* %arrayidx15, align 8
  %24 = load i64, i64* %lo_bit_count, align 8
  %25 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %data16 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %25, i32 0, i32 3
  %arrayidx17 = getelementptr inbounds [16 x i64], [16 x i64]* %data16, i64 0, i64 15
  store i64 %24, i64* %arrayidx17, align 8
  %26 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  call void @sha_transform(%struct.SHA_INFO* %26)
  ret void
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @sha_stream(%struct.SHA_INFO* %sha_info, %struct._IO_FILE* %fin) #0 {
entry:
  %sha_info.addr = alloca %struct.SHA_INFO*, align 8
  %fin.addr = alloca %struct._IO_FILE*, align 8
  %i = alloca i32, align 4
  %data = alloca [8192 x i8], align 16
  store %struct.SHA_INFO* %sha_info, %struct.SHA_INFO** %sha_info.addr, align 8
  store %struct._IO_FILE* %fin, %struct._IO_FILE** %fin.addr, align 8
  %0 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  call void @sha_init(%struct.SHA_INFO* %0)
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %arraydecay = getelementptr inbounds [8192 x i8], [8192 x i8]* %data, i64 0, i64 0
  %1 = load %struct._IO_FILE*, %struct._IO_FILE** %fin.addr, align 8
  %call = call i64 @fread(i8* %arraydecay, i64 1, i64 8192, %struct._IO_FILE* %1)
  %conv = trunc i64 %call to i32
  store i32 %conv, i32* %i, align 4
  %cmp = icmp sgt i32 %conv, 0
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %2 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %arraydecay2 = getelementptr inbounds [8192 x i8], [8192 x i8]* %data, i64 0, i64 0
  %3 = load i32, i32* %i, align 4
  call void @sha_update(%struct.SHA_INFO* %2, i8* %arraydecay2, i32 %3)
  br label %while.cond, !llvm.loop !11

while.end:                                        ; preds = %while.cond
  %4 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  call void @sha_final(%struct.SHA_INFO* %4)
  ret void
}

declare dso_local i64 @fread(i8*, i64, i64, %struct._IO_FILE*) #3

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @sha_print(%struct.SHA_INFO* %sha_info) #0 {
entry:
  %sha_info.addr = alloca %struct.SHA_INFO*, align 8
  store %struct.SHA_INFO* %sha_info, %struct.SHA_INFO** %sha_info.addr, align 8
  %0 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %0, i32 0, i32 0
  %arrayidx = getelementptr inbounds [5 x i64], [5 x i64]* %digest, i64 0, i64 0
  %1 = load i64, i64* %arrayidx, align 8
  %2 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest1 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %2, i32 0, i32 0
  %arrayidx2 = getelementptr inbounds [5 x i64], [5 x i64]* %digest1, i64 0, i64 1
  %3 = load i64, i64* %arrayidx2, align 8
  %4 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest3 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %4, i32 0, i32 0
  %arrayidx4 = getelementptr inbounds [5 x i64], [5 x i64]* %digest3, i64 0, i64 2
  %5 = load i64, i64* %arrayidx4, align 8
  %6 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest5 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %6, i32 0, i32 0
  %arrayidx6 = getelementptr inbounds [5 x i64], [5 x i64]* %digest5, i64 0, i64 3
  %7 = load i64, i64* %arrayidx6, align 8
  %8 = load %struct.SHA_INFO*, %struct.SHA_INFO** %sha_info.addr, align 8
  %digest7 = getelementptr inbounds %struct.SHA_INFO, %struct.SHA_INFO* %8, i32 0, i32 0
  %arrayidx8 = getelementptr inbounds [5 x i64], [5 x i64]* %digest7, i64 0, i64 4
  %9 = load i64, i64* %arrayidx8, align 8
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str, i64 0, i64 0), i64 %1, i64 %3, i64 %5, i64 %7, i64 %9)
  ret void
}

declare dso_local i32 @printf(i8*, ...) #3

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { argmemonly nofree nosync nounwind willreturn }
attributes #2 = { argmemonly nofree nosync nounwind willreturn writeonly }
attributes #3 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

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
!8 = distinct !{!8, !3}
!9 = distinct !{!9, !3}
!10 = distinct !{!10, !3}
!11 = distinct !{!11, !3}
