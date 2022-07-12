; ModuleID = 'sha_driver.c'
source_filename = "sha_driver.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }
%struct.SHA_INFO = type { [5 x i64], i64, i64, [16 x i64] }

@stdin = external dso_local global %struct._IO_FILE*, align 8
@.str = private unnamed_addr constant [3 x i8] c"rb\00", align 1
@.str.1 = private unnamed_addr constant [30 x i8] c"error opening %s for reading\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %fin = alloca %struct._IO_FILE*, align 8
  %sha_info = alloca %struct.SHA_INFO, align 8
  store i32 0, i32* %retval, align 4
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  %0 = load i32, i32* %argc.addr, align 4
  %cmp = icmp slt i32 %0, 2
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  %1 = load %struct._IO_FILE*, %struct._IO_FILE** @stdin, align 8
  store %struct._IO_FILE* %1, %struct._IO_FILE** %fin, align 8
  %2 = load %struct._IO_FILE*, %struct._IO_FILE** %fin, align 8
  call void @sha_stream(%struct.SHA_INFO* %sha_info, %struct._IO_FILE* %2)
  call void @sha_print(%struct.SHA_INFO* %sha_info)
  br label %if.end6

if.else:                                          ; preds = %entry
  br label %while.cond

while.cond:                                       ; preds = %if.end, %if.else
  %3 = load i32, i32* %argc.addr, align 4
  %dec = add nsw i32 %3, -1
  store i32 %dec, i32* %argc.addr, align 4
  %tobool = icmp ne i32 %dec, 0
  br i1 %tobool, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %4 = load i8**, i8*** %argv.addr, align 8
  %incdec.ptr = getelementptr inbounds i8*, i8** %4, i32 1
  store i8** %incdec.ptr, i8*** %argv.addr, align 8
  %5 = load i8*, i8** %incdec.ptr, align 8
  %call = call %struct._IO_FILE* @fopen(i8* %5, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0))
  store %struct._IO_FILE* %call, %struct._IO_FILE** %fin, align 8
  %6 = load %struct._IO_FILE*, %struct._IO_FILE** %fin, align 8
  %cmp1 = icmp eq %struct._IO_FILE* %6, null
  br i1 %cmp1, label %if.then2, label %if.else4

if.then2:                                         ; preds = %while.body
  %7 = load i8**, i8*** %argv.addr, align 8
  %8 = load i8*, i8** %7, align 8
  %call3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([30 x i8], [30 x i8]* @.str.1, i64 0, i64 0), i8* %8)
  br label %if.end

if.else4:                                         ; preds = %while.body
  %9 = load %struct._IO_FILE*, %struct._IO_FILE** %fin, align 8
  call void @sha_stream(%struct.SHA_INFO* %sha_info, %struct._IO_FILE* %9)
  call void @sha_print(%struct.SHA_INFO* %sha_info)
  %10 = load %struct._IO_FILE*, %struct._IO_FILE** %fin, align 8
  %call5 = call i32 @fclose(%struct._IO_FILE* %10)
  br label %if.end

if.end:                                           ; preds = %if.else4, %if.then2
  br label %while.cond, !llvm.loop !2

while.end:                                        ; preds = %while.cond
  br label %if.end6

if.end6:                                          ; preds = %while.end, %if.then
  ret i32 0
}

declare dso_local void @sha_stream(%struct.SHA_INFO*, %struct._IO_FILE*) #1

declare dso_local void @sha_print(%struct.SHA_INFO*) #1

declare dso_local %struct._IO_FILE* @fopen(i8*, i8*) #1

declare dso_local i32 @printf(i8*, ...) #1

declare dso_local i32 @fclose(%struct._IO_FILE*) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 13.0.0 (https://github.com/llvm/llvm-project.git f5ba3eea6746559513af7ed32db8083ad52661a3)"}
!2 = distinct !{!2, !3}
!3 = !{!"llvm.loop.mustprogress"}
