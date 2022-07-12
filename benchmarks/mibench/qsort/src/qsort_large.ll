; ModuleID = 'qsort_large.c'
source_filename = "qsort_large.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }
%struct.my3DVertexStruct = type { i32, i32, i32, double }

@stderr = external dso_local global %struct._IO_FILE*, align 8
@.str = private unnamed_addr constant [27 x i8] c"Usage: qsort_large <file>\0A\00", align 1
@.str.1 = private unnamed_addr constant [2 x i8] c"r\00", align 1
@.str.2 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.3 = private unnamed_addr constant [57 x i8] c"\0ASorting %d vectors based on distance from the origin.\0A\0A\00", align 1
@.str.4 = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @compare(i8* %elem1, i8* %elem2) #0 {
entry:
  %elem1.addr = alloca i8*, align 8
  %elem2.addr = alloca i8*, align 8
  %distance1 = alloca double, align 8
  %distance2 = alloca double, align 8
  store i8* %elem1, i8** %elem1.addr, align 8
  store i8* %elem2, i8** %elem2.addr, align 8
  %0 = load i8*, i8** %elem1.addr, align 8
  %1 = bitcast i8* %0 to %struct.my3DVertexStruct*
  %distance = getelementptr inbounds %struct.my3DVertexStruct, %struct.my3DVertexStruct* %1, i32 0, i32 3
  %2 = load double, double* %distance, align 8
  store double %2, double* %distance1, align 8
  %3 = load i8*, i8** %elem2.addr, align 8
  %4 = bitcast i8* %3 to %struct.my3DVertexStruct*
  %distance3 = getelementptr inbounds %struct.my3DVertexStruct, %struct.my3DVertexStruct* %4, i32 0, i32 3
  %5 = load double, double* %distance3, align 8
  store double %5, double* %distance2, align 8
  %6 = load double, double* %distance1, align 8
  %7 = load double, double* %distance2, align 8
  %cmp = fcmp ogt double %6, %7
  br i1 %cmp, label %cond.true, label %cond.false

cond.true:                                        ; preds = %entry
  br label %cond.end

cond.false:                                       ; preds = %entry
  %8 = load double, double* %distance1, align 8
  %9 = load double, double* %distance2, align 8
  %cmp4 = fcmp oeq double %8, %9
  %10 = zext i1 %cmp4 to i64
  %cond = select i1 %cmp4, i32 0, i32 -1
  br label %cond.end

cond.end:                                         ; preds = %cond.false, %cond.true
  %cond5 = phi i32 [ 1, %cond.true ], [ %cond, %cond.false ]
  ret i32 %cond5
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %array = alloca [60000 x %struct.my3DVertexStruct], align 16
  %fp = alloca %struct._IO_FILE*, align 8
  %i = alloca i32, align 4
  %count = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  store i32 0, i32* %count, align 4
  %0 = load i32, i32* %argc.addr, align 4
  %cmp = icmp slt i32 %0, 2
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %1 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8
  %call = call i32 (%struct._IO_FILE*, i8*, ...) @fprintf(%struct._IO_FILE* %1, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str, i64 0, i64 0))
  call void @exit(i32 -1) #4
  unreachable

if.else:                                          ; preds = %entry
  %2 = load i8**, i8*** %argv.addr, align 8
  %arrayidx = getelementptr inbounds i8*, i8** %2, i64 1
  %3 = load i8*, i8** %arrayidx, align 8
  %call1 = call %struct._IO_FILE* @fopen(i8* %3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.1, i64 0, i64 0))
  store %struct._IO_FILE* %call1, %struct._IO_FILE** %fp, align 8
  br label %while.cond

while.cond:                                       ; preds = %while.body, %if.else
  %4 = load %struct._IO_FILE*, %struct._IO_FILE** %fp, align 8
  %call2 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %4, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i64 0, i64 0), i32* %x)
  %cmp3 = icmp eq i32 %call2, 1
  br i1 %cmp3, label %land.lhs.true, label %land.end

land.lhs.true:                                    ; preds = %while.cond
  %5 = load %struct._IO_FILE*, %struct._IO_FILE** %fp, align 8
  %call4 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %5, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i64 0, i64 0), i32* %y)
  %cmp5 = icmp eq i32 %call4, 1
  br i1 %cmp5, label %land.lhs.true6, label %land.end

land.lhs.true6:                                   ; preds = %land.lhs.true
  %6 = load %struct._IO_FILE*, %struct._IO_FILE** %fp, align 8
  %call7 = call i32 (%struct._IO_FILE*, i8*, ...) @__isoc99_fscanf(%struct._IO_FILE* %6, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.2, i64 0, i64 0), i32* %z)
  %cmp8 = icmp eq i32 %call7, 1
  br i1 %cmp8, label %land.rhs, label %land.end

land.rhs:                                         ; preds = %land.lhs.true6
  %7 = load i32, i32* %count, align 4
  %cmp9 = icmp slt i32 %7, 60000
  br label %land.end

land.end:                                         ; preds = %land.rhs, %land.lhs.true6, %land.lhs.true, %while.cond
  %8 = phi i1 [ false, %land.lhs.true6 ], [ false, %land.lhs.true ], [ false, %while.cond ], [ %cmp9, %land.rhs ]
  br i1 %8, label %while.body, label %while.end

while.body:                                       ; preds = %land.end
  %9 = load i32, i32* %x, align 4
  %10 = load i32, i32* %count, align 4
  %idxprom = sext i32 %10 to i64
  %arrayidx10 = getelementptr inbounds [60000 x %struct.my3DVertexStruct], [60000 x %struct.my3DVertexStruct]* %array, i64 0, i64 %idxprom
  %x11 = getelementptr inbounds %struct.my3DVertexStruct, %struct.my3DVertexStruct* %arrayidx10, i32 0, i32 0
  store i32 %9, i32* %x11, align 8
  %11 = load i32, i32* %y, align 4
  %12 = load i32, i32* %count, align 4
  %idxprom12 = sext i32 %12 to i64
  %arrayidx13 = getelementptr inbounds [60000 x %struct.my3DVertexStruct], [60000 x %struct.my3DVertexStruct]* %array, i64 0, i64 %idxprom12
  %y14 = getelementptr inbounds %struct.my3DVertexStruct, %struct.my3DVertexStruct* %arrayidx13, i32 0, i32 1
  store i32 %11, i32* %y14, align 4
  %13 = load i32, i32* %z, align 4
  %14 = load i32, i32* %count, align 4
  %idxprom15 = sext i32 %14 to i64
  %arrayidx16 = getelementptr inbounds [60000 x %struct.my3DVertexStruct], [60000 x %struct.my3DVertexStruct]* %array, i64 0, i64 %idxprom15
  %z17 = getelementptr inbounds %struct.my3DVertexStruct, %struct.my3DVertexStruct* %arrayidx16, i32 0, i32 2
  store i32 %13, i32* %z17, align 8
  %15 = load i32, i32* %x, align 4
  %conv = sitofp i32 %15 to double
  %call18 = call double @pow(double %conv, double 2.000000e+00) #5
  %16 = load i32, i32* %y, align 4
  %conv19 = sitofp i32 %16 to double
  %call20 = call double @pow(double %conv19, double 2.000000e+00) #5
  %add = fadd double %call18, %call20
  %17 = load i32, i32* %z, align 4
  %conv21 = sitofp i32 %17 to double
  %call22 = call double @pow(double %conv21, double 2.000000e+00) #5
  %add23 = fadd double %add, %call22
  %call24 = call double @sqrt(double %add23) #5
  %18 = load i32, i32* %count, align 4
  %idxprom25 = sext i32 %18 to i64
  %arrayidx26 = getelementptr inbounds [60000 x %struct.my3DVertexStruct], [60000 x %struct.my3DVertexStruct]* %array, i64 0, i64 %idxprom25
  %distance = getelementptr inbounds %struct.my3DVertexStruct, %struct.my3DVertexStruct* %arrayidx26, i32 0, i32 3
  store double %call24, double* %distance, align 8
  %19 = load i32, i32* %count, align 4
  %inc = add nsw i32 %19, 1
  store i32 %inc, i32* %count, align 4
  br label %while.cond, !llvm.loop !2

while.end:                                        ; preds = %land.end
  br label %if.end

if.end:                                           ; preds = %while.end
  %20 = load i32, i32* %count, align 4
  %call27 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([57 x i8], [57 x i8]* @.str.3, i64 0, i64 0), i32 %20)
  %arraydecay = getelementptr inbounds [60000 x %struct.my3DVertexStruct], [60000 x %struct.my3DVertexStruct]* %array, i64 0, i64 0
  %21 = bitcast %struct.my3DVertexStruct* %arraydecay to i8*
  %22 = load i32, i32* %count, align 4
  %conv28 = sext i32 %22 to i64
  call void @qsort(i8* %21, i64 %conv28, i64 24, i32 (i8*, i8*)* @compare)
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end
  %23 = load i32, i32* %i, align 4
  %24 = load i32, i32* %count, align 4
  %cmp29 = icmp slt i32 %23, %24
  br i1 %cmp29, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %25 = load i32, i32* %i, align 4
  %idxprom31 = sext i32 %25 to i64
  %arrayidx32 = getelementptr inbounds [60000 x %struct.my3DVertexStruct], [60000 x %struct.my3DVertexStruct]* %array, i64 0, i64 %idxprom31
  %x33 = getelementptr inbounds %struct.my3DVertexStruct, %struct.my3DVertexStruct* %arrayidx32, i32 0, i32 0
  %26 = load i32, i32* %x33, align 8
  %27 = load i32, i32* %i, align 4
  %idxprom34 = sext i32 %27 to i64
  %arrayidx35 = getelementptr inbounds [60000 x %struct.my3DVertexStruct], [60000 x %struct.my3DVertexStruct]* %array, i64 0, i64 %idxprom34
  %y36 = getelementptr inbounds %struct.my3DVertexStruct, %struct.my3DVertexStruct* %arrayidx35, i32 0, i32 1
  %28 = load i32, i32* %y36, align 4
  %29 = load i32, i32* %i, align 4
  %idxprom37 = sext i32 %29 to i64
  %arrayidx38 = getelementptr inbounds [60000 x %struct.my3DVertexStruct], [60000 x %struct.my3DVertexStruct]* %array, i64 0, i64 %idxprom37
  %z39 = getelementptr inbounds %struct.my3DVertexStruct, %struct.my3DVertexStruct* %arrayidx38, i32 0, i32 2
  %30 = load i32, i32* %z39, align 8
  %call40 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.4, i64 0, i64 0), i32 %26, i32 %28, i32 %30)
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %31 = load i32, i32* %i, align 4
  %inc41 = add nsw i32 %31, 1
  store i32 %inc41, i32* %i, align 4
  br label %for.cond, !llvm.loop !4

for.end:                                          ; preds = %for.cond
  ret i32 0
}

declare dso_local i32 @fprintf(%struct._IO_FILE*, i8*, ...) #1

; Function Attrs: noreturn nounwind
declare dso_local void @exit(i32) #2

declare dso_local %struct._IO_FILE* @fopen(i8*, i8*) #1

declare dso_local i32 @__isoc99_fscanf(%struct._IO_FILE*, i8*, ...) #1

; Function Attrs: nounwind
declare dso_local double @sqrt(double) #3

; Function Attrs: nounwind
declare dso_local double @pow(double, double) #3

declare dso_local i32 @printf(i8*, ...) #1

declare dso_local void @qsort(i8*, i64, i64, i32 (i8*, i8*)*) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { noreturn nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { noreturn nounwind }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 13.0.0 (https://github.com/llvm/llvm-project.git f5ba3eea6746559513af7ed32db8083ad52661a3)"}
!2 = distinct !{!2, !3}
!3 = !{!"llvm.loop.mustprogress"}
!4 = distinct !{!4, !3}
