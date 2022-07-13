; ModuleID = 'main'
source_filename = "main"

@stepsizeTable = internal global [89 x i32] [i32 7, i32 8, i32 9, i32 10, i32 11, i32 12, i32 13, i32 14, i32 16, i32 17, i32 19, i32 21, i32 23, i32 25, i32 28, i32 31, i32 34, i32 37, i32 41, i32 45, i32 50, i32 55, i32 60, i32 66, i32 73, i32 80, i32 88, i32 97, i32 107, i32 118, i32 130, i32 143, i32 157, i32 173, i32 190, i32 209, i32 230, i32 253, i32 279, i32 307, i32 337, i32 371, i32 408, i32 449, i32 494, i32 544, i32 598, i32 658, i32 724, i32 796, i32 876, i32 963, i32 1060, i32 1166, i32 1282, i32 1411, i32 1552, i32 1707, i32 1878, i32 2066, i32 2272, i32 2499, i32 2749, i32 3024, i32 3327, i32 3660, i32 4026, i32 4428, i32 4871, i32 5358, i32 5894, i32 6484, i32 7132, i32 7845, i32 8630, i32 9493, i32 10442, i32 11487, i32 12635, i32 13899, i32 15289, i32 16818, i32 18500, i32 20350, i32 22385, i32 24623, i32 27086, i32 29794, i32 32767], align 4
@indexTable = internal global [16 x i32] [i32 -1, i32 -1, i32 -1, i32 -1, i32 2, i32 4, i32 6, i32 8, i32 -1, i32 -1, i32 -1, i32 -1, i32 2, i32 4, i32 6, i32 8], align 4
@rodata_14 = private unnamed_addr constant [30 x i8] c"\01\00\02\00CT_REPEAT_MAIN\00input file\00", align 4, !ROData_SecInfo !0
@state = common dso_local global i32 0, align 4
@sbuf = common dso_local global [2000 x i8] zeroinitializer, align 16
@abuf = common dso_local global [500 x i8] zeroinitializer, align 16

define dso_local i64 @adpcm_coder(i64 %arg1, i64 %arg2, i32 %arg3, i64 %arg4) {
entry:
  %stktop_4 = alloca i8, i32 96, align 1
  %RBP_N.88 = bitcast i8* %stktop_4 to i32*
  %tos = ptrtoint i8* %stktop_4 to i64
  %0 = add i64 %tos, 12
  %RBP_N.84 = inttoptr i64 %0 to i32*
  %1 = add i64 %tos, 16
  %RBP_N.80 = inttoptr i64 %1 to i32*
  %2 = add i64 %tos, 20
  %RBP_N.76 = inttoptr i64 %2 to i32*
  %3 = add i64 %tos, 24
  %RBP_N.72 = inttoptr i64 %3 to i32*
  %4 = add i64 %tos, 28
  %RBP_N.68 = inttoptr i64 %4 to i32*
  %5 = add i64 %tos, 32
  %RBP_N.64 = inttoptr i64 %5 to i32*
  %6 = add i64 %tos, 36
  %RBP_N.60 = inttoptr i64 %6 to i32*
  %7 = add i64 %tos, 40
  %RBP_N.56 = inttoptr i64 %7 to i32*
  %8 = add i64 %tos, 44
  %RBP_N.52 = inttoptr i64 %8 to i32*
  %9 = add i64 %tos, 48
  %RBP_N.48 = inttoptr i64 %9 to i64*
  %10 = add i64 %tos, 56
  %RBP_N.40 = inttoptr i64 %10 to i64*
  %11 = add i64 %tos, 64
  %RBP_N.32 = inttoptr i64 %11 to i64*
  %12 = add i64 %tos, 76
  %RBP_N.20 = inttoptr i64 %12 to i32*
  %13 = add i64 %tos, 80
  %RBP_N.16 = inttoptr i64 %13 to i64*
  %14 = add i64 %tos, 88
  %RBP_N.8 = inttoptr i64 %14 to i64*
  %15 = add i64 %tos, 0
  %RSP_P.0 = inttoptr i64 %15 to i64*
  %RBP = ptrtoint i64* %RSP_P.0 to i64
  store i64 %arg1, i64* %RBP_N.8, align 1
  store i64 %arg2, i64* %RBP_N.16, align 1
  store i32 %arg3, i32* %RBP_N.20, align 1
  store i64 %arg4, i64* %RBP_N.32, align 1
  store i32 0, i32* %RBP_N.84, align 1
  %memload = load i64, i64* %RBP_N.16, align 1
  store i64 %memload, i64* %RBP_N.48, align 1
  %memload1 = load i64, i64* %RBP_N.8, align 1
  store i64 %memload1, i64* %RBP_N.40, align 1
  %memload2 = load i64, i64* %RBP_N.32, align 1
  %16 = inttoptr i64 %memload2 to i32*
  %memload3 = load i32, i32* %16, align 1
  %17 = trunc i32 %memload3 to i16
  %EAX = sext i16 %17 to i32
  store i32 %EAX, i32* %RBP_N.72, align 1
  %memload4 = load i64, i64* %RBP_N.32, align 1
  %memref-disp = add i64 %memload4, 2
  %18 = inttoptr i64 %memref-disp to i32*
  %memload5 = load i32, i32* %18, align 1
  %19 = trunc i32 %memload5 to i8
  %EAX6 = sext i8 %19 to i32
  store i32 %EAX6, i32* %RBP_N.80, align 1
  %20 = bitcast i32* %RBP_N.80 to i64*
  %memload7 = load i64, i64* %20, align 1
  %21 = trunc i64 %memload7 to i32
  %RAX = sext i32 %21 to i64
  %memref-idxreg = mul i64 4, %RAX
  %22 = getelementptr inbounds [89 x i32], [89 x i32]* @stepsizeTable, i32 0, i32 0
  %23 = bitcast [89 x i32]* @stepsizeTable to [356 x i8]*
  %24 = getelementptr inbounds [356 x i8], [356 x i8]* %23, i32 0, i32 0
  %25 = ptrtoint i8* %24 to i64
  %memref-disp8 = add i64 %memref-idxreg, %25
  %26 = inttoptr i64 %memref-disp8 to i32*
  %memload9 = load i32, i32* %26, align 1
  store i32 %memload9, i32* %RBP_N.68, align 1
  store i32 1, i32* %RBP_N.88, align 1
  br label %bb.1

bb.1:                                             ; preds = %entry, %bb.25
  %27 = load i32, i32* %RBP_N.20, align 1
  %28 = zext i32 %27 to i64
  %29 = zext i32 0 to i64
  %30 = sub i64 %28, %29
  %31 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %28, i64 %29)
  %CF = extractvalue { i64, i1 } %31, 1
  %ZF = icmp eq i64 %30, 0
  %highbit = and i64 -9223372036854775808, %30
  %SF = icmp ne i64 %highbit, 0
  %32 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %28, i64 %29)
  %OF = extractvalue { i64, i1 } %32, 1
  %CmpZF_JLE = icmp eq i1 %ZF, true
  %CmpOF_JLE = icmp ne i1 %SF, %OF
  %ZFOrSF_JLE = or i1 %CmpZF_JLE, %CmpOF_JLE
  br i1 %ZFOrSF_JLE, label %bb.26, label %bb.2

bb.2:                                             ; preds = %bb.1
  %memload10 = load i64, i64* %RBP_N.40, align 1
  %RCX = add i64 %memload10, 2
  %33 = call { i64, i1 } @llvm.uadd.with.overflow.i64(i64 %memload10, i64 2)
  %CF11 = extractvalue { i64, i1 } %33, 1
  %ZF12 = icmp eq i64 %RCX, 0
  %highbit13 = and i64 -9223372036854775808, %RCX
  %SF14 = icmp ne i64 %highbit13, 0
  %34 = call { i64, i1 } @llvm.sadd.with.overflow.i64(i64 %memload10, i64 2)
  %OF15 = extractvalue { i64, i1 } %34, 1
  store i64 %RCX, i64* %RBP_N.40, align 1
  %35 = inttoptr i64 %memload10 to i32*
  %memload16 = load i32, i32* %35, align 1
  %36 = trunc i32 %memload16 to i16
  %EAX17 = sext i16 %36 to i32
  store i32 %EAX17, i32* %RBP_N.52, align 1
  %memload18 = load i32, i32* %RBP_N.52, align 1
  %37 = load i32, i32* %RBP_N.72, align 1
  %EAX19 = sub i32 %memload18, %37
  %38 = call { i32, i1 } @llvm.usub.with.overflow.i32(i32 %memload18, i32 %37)
  %CF20 = extractvalue { i32, i1 } %38, 1
  %ZF21 = icmp eq i32 %EAX19, 0
  %highbit22 = and i32 -2147483648, %EAX19
  %SF23 = icmp ne i32 %highbit22, 0
  %39 = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 %memload18, i32 %37)
  %OF24 = extractvalue { i32, i1 } %39, 1
  store i32 %EAX19, i32* %RBP_N.64, align 1
  %memload25 = load i32, i32* %RBP_N.64, align 1
  %40 = sub i32 %memload25, 0
  %41 = call { i32, i1 } @llvm.usub.with.overflow.i32(i32 %memload25, i32 0)
  %CF26 = extractvalue { i32, i1 } %41, 1
  %ZF27 = icmp eq i32 %40, 0
  %highbit28 = and i32 -2147483648, %40
  %SF29 = icmp ne i32 %highbit28, 0
  %42 = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 %memload25, i32 0)
  %OF30 = extractvalue { i32, i1 } %42, 1
  %Cond_CMOVL = icmp ne i1 %SF29, %OF30
  %CMOV = select i1 %Cond_CMOVL, i32 8, i32 0
  store i32 %CMOV, i32* %RBP_N.56, align 1
  %43 = load i32, i32* %RBP_N.56, align 1
  %44 = zext i32 %43 to i64
  %45 = zext i32 0 to i64
  %46 = sub i64 %44, %45
  %47 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %44, i64 %45)
  %CF31 = extractvalue { i64, i1 } %47, 1
  %ZF32 = icmp eq i64 %46, 0
  %highbit33 = and i64 -9223372036854775808, %46
  %SF34 = icmp ne i64 %highbit33, 0
  %48 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %44, i64 %45)
  %OF35 = extractvalue { i64, i1 } %48, 1
  %CmpZF_JE = icmp eq i1 %ZF32, true
  br i1 %CmpZF_JE, label %bb.4, label %bb.3

bb.3:                                             ; preds = %bb.2
  %49 = load i32, i32* %RBP_N.64, align 1
  %EAX36 = sub i32 0, %49
  %50 = call { i32, i1 } @llvm.usub.with.overflow.i32(i32 0, i32 %49)
  %CF37 = extractvalue { i32, i1 } %50, 1
  %ZF38 = icmp eq i32 %EAX36, 0
  %highbit39 = and i32 -2147483648, %EAX36
  %SF40 = icmp ne i32 %highbit39, 0
  %51 = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 0, i32 %49)
  %OF41 = extractvalue { i32, i1 } %51, 1
  store i32 %EAX36, i32* %RBP_N.64, align 1
  br label %bb.4

bb.4:                                             ; preds = %bb.3, %bb.2
  store i32 0, i32* %RBP_N.60, align 1
  %memload42 = load i32, i32* %RBP_N.68, align 1
  %EAX46 = lshr i32 %memload42, 3
  %ZF43 = icmp eq i32 %EAX46, 0
  %highbit44 = and i32 -2147483648, %EAX46
  %SF45 = icmp ne i32 %highbit44, 0
  store i32 %EAX46, i32* %RBP_N.76, align 1
  %memload47 = load i32, i32* %RBP_N.64, align 1
  %52 = load i32, i32* %RBP_N.68, align 1
  %53 = sub i32 %memload47, %52
  %54 = call { i32, i1 } @llvm.usub.with.overflow.i32(i32 %memload47, i32 %52)
  %CF48 = extractvalue { i32, i1 } %54, 1
  %ZF49 = icmp eq i32 %53, 0
  %highbit50 = and i32 -2147483648, %53
  %SF51 = icmp ne i32 %highbit50, 0
  %55 = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 %memload47, i32 %52)
  %OF52 = extractvalue { i32, i1 } %55, 1
  %SFAndOF_JL = icmp ne i1 %SF51, %OF52
  br i1 %SFAndOF_JL, label %bb.6, label %bb.5

bb.5:                                             ; preds = %bb.4
  store i32 4, i32* %RBP_N.60, align 1
  %memload53 = load i32, i32* %RBP_N.68, align 1
  %memload54 = load i32, i32* %RBP_N.64, align 1
  %EAX55 = sub i32 %memload54, %memload53
  %56 = call { i32, i1 } @llvm.usub.with.overflow.i32(i32 %memload54, i32 %memload53)
  %CF56 = extractvalue { i32, i1 } %56, 1
  %ZF57 = icmp eq i32 %EAX55, 0
  %highbit58 = and i32 -2147483648, %EAX55
  %SF59 = icmp ne i32 %highbit58, 0
  %57 = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 %memload54, i32 %memload53)
  %OF60 = extractvalue { i32, i1 } %57, 1
  store i32 %EAX55, i32* %RBP_N.64, align 1
  %memload61 = load i32, i32* %RBP_N.68, align 1
  %memload62 = load i32, i32* %RBP_N.76, align 4
  %EAX63 = add i32 %memload61, %memload62
  store i32 %EAX63, i32* %RBP_N.76, align 1
  br label %bb.6

bb.6:                                             ; preds = %bb.5, %bb.4
  %memload64 = load i32, i32* %RBP_N.68, align 1
  %EAX68 = lshr i32 %memload64, 1
  %ZF65 = icmp eq i32 %EAX68, 0
  %highbit66 = and i32 -2147483648, %EAX68
  %SF67 = icmp ne i32 %highbit66, 0
  store i32 %EAX68, i32* %RBP_N.68, align 1
  %memload69 = load i32, i32* %RBP_N.64, align 1
  %58 = load i32, i32* %RBP_N.68, align 1
  %59 = sub i32 %memload69, %58
  %60 = call { i32, i1 } @llvm.usub.with.overflow.i32(i32 %memload69, i32 %58)
  %CF70 = extractvalue { i32, i1 } %60, 1
  %ZF71 = icmp eq i32 %59, 0
  %highbit72 = and i32 -2147483648, %59
  %SF73 = icmp ne i32 %highbit72, 0
  %61 = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 %memload69, i32 %58)
  %OF74 = extractvalue { i32, i1 } %61, 1
  %SFAndOF_JL229 = icmp ne i1 %SF73, %OF74
  br i1 %SFAndOF_JL229, label %bb.8, label %bb.7

bb.7:                                             ; preds = %bb.6
  %memload75 = load i32, i32* %RBP_N.60, align 1
  %EAX79 = or i32 %memload75, 2
  %ZF76 = icmp eq i32 %EAX79, 0
  %highbit77 = and i32 -2147483648, %EAX79
  %SF78 = icmp ne i32 %highbit77, 0
  store i32 %EAX79, i32* %RBP_N.60, align 1
  %memload80 = load i32, i32* %RBP_N.68, align 1
  %memload81 = load i32, i32* %RBP_N.64, align 1
  %EAX82 = sub i32 %memload81, %memload80
  %62 = call { i32, i1 } @llvm.usub.with.overflow.i32(i32 %memload81, i32 %memload80)
  %CF83 = extractvalue { i32, i1 } %62, 1
  %ZF84 = icmp eq i32 %EAX82, 0
  %highbit85 = and i32 -2147483648, %EAX82
  %SF86 = icmp ne i32 %highbit85, 0
  %63 = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 %memload81, i32 %memload80)
  %OF87 = extractvalue { i32, i1 } %63, 1
  store i32 %EAX82, i32* %RBP_N.64, align 1
  %memload88 = load i32, i32* %RBP_N.68, align 1
  %memload89 = load i32, i32* %RBP_N.76, align 4
  %EAX90 = add i32 %memload88, %memload89
  store i32 %EAX90, i32* %RBP_N.76, align 1
  br label %bb.8

bb.8:                                             ; preds = %bb.7, %bb.6
  %memload91 = load i32, i32* %RBP_N.68, align 1
  %EAX95 = lshr i32 %memload91, 1
  %ZF92 = icmp eq i32 %EAX95, 0
  %highbit93 = and i32 -2147483648, %EAX95
  %SF94 = icmp ne i32 %highbit93, 0
  store i32 %EAX95, i32* %RBP_N.68, align 1
  %memload96 = load i32, i32* %RBP_N.64, align 1
  %64 = load i32, i32* %RBP_N.68, align 1
  %65 = sub i32 %memload96, %64
  %66 = call { i32, i1 } @llvm.usub.with.overflow.i32(i32 %memload96, i32 %64)
  %CF97 = extractvalue { i32, i1 } %66, 1
  %ZF98 = icmp eq i32 %65, 0
  %highbit99 = and i32 -2147483648, %65
  %SF100 = icmp ne i32 %highbit99, 0
  %67 = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 %memload96, i32 %64)
  %OF101 = extractvalue { i32, i1 } %67, 1
  %SFAndOF_JL230 = icmp ne i1 %SF100, %OF101
  br i1 %SFAndOF_JL230, label %bb.10, label %bb.9

bb.9:                                             ; preds = %bb.8
  %memload102 = load i32, i32* %RBP_N.60, align 1
  %EAX106 = or i32 %memload102, 1
  %ZF103 = icmp eq i32 %EAX106, 0
  %highbit104 = and i32 -2147483648, %EAX106
  %SF105 = icmp ne i32 %highbit104, 0
  store i32 %EAX106, i32* %RBP_N.60, align 1
  %memload107 = load i32, i32* %RBP_N.68, align 1
  %memload108 = load i32, i32* %RBP_N.76, align 4
  %EAX109 = add i32 %memload107, %memload108
  store i32 %EAX109, i32* %RBP_N.76, align 1
  br label %bb.10

bb.10:                                            ; preds = %bb.9, %bb.8
  %68 = load i32, i32* %RBP_N.56, align 1
  %69 = zext i32 %68 to i64
  %70 = zext i32 0 to i64
  %71 = sub i64 %69, %70
  %72 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %69, i64 %70)
  %CF110 = extractvalue { i64, i1 } %72, 1
  %ZF111 = icmp eq i64 %71, 0
  %highbit112 = and i64 -9223372036854775808, %71
  %SF113 = icmp ne i64 %highbit112, 0
  %73 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %69, i64 %70)
  %OF114 = extractvalue { i64, i1 } %73, 1
  %CmpZF_JE231 = icmp eq i1 %ZF111, true
  br i1 %CmpZF_JE231, label %bb.12, label %bb.11

bb.11:                                            ; preds = %bb.10
  %memload115 = load i32, i32* %RBP_N.76, align 1
  %memload116 = load i32, i32* %RBP_N.72, align 1
  %EAX117 = sub i32 %memload116, %memload115
  %74 = call { i32, i1 } @llvm.usub.with.overflow.i32(i32 %memload116, i32 %memload115)
  %CF118 = extractvalue { i32, i1 } %74, 1
  %ZF119 = icmp eq i32 %EAX117, 0
  %highbit120 = and i32 -2147483648, %EAX117
  %SF121 = icmp ne i32 %highbit120, 0
  %75 = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 %memload116, i32 %memload115)
  %OF122 = extractvalue { i32, i1 } %75, 1
  store i32 %EAX117, i32* %RBP_N.72, align 1
  br label %bb.13

bb.12:                                            ; preds = %bb.10
  %memload123 = load i32, i32* %RBP_N.76, align 1
  %memload124 = load i32, i32* %RBP_N.72, align 4
  %EAX125 = add i32 %memload123, %memload124
  store i32 %EAX125, i32* %RBP_N.72, align 1
  br label %bb.13

bb.13:                                            ; preds = %bb.12, %bb.11
  %76 = load i32, i32* %RBP_N.72, align 1
  %77 = zext i32 %76 to i64
  %78 = zext i32 32767 to i64
  %79 = sub i64 %77, %78
  %80 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %77, i64 %78)
  %CF126 = extractvalue { i64, i1 } %80, 1
  %ZF127 = icmp eq i64 %79, 0
  %highbit128 = and i64 -9223372036854775808, %79
  %SF129 = icmp ne i64 %highbit128, 0
  %81 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %77, i64 %78)
  %OF130 = extractvalue { i64, i1 } %81, 1
  %CmpZF_JLE232 = icmp eq i1 %ZF127, true
  %CmpOF_JLE233 = icmp ne i1 %SF129, %OF130
  %ZFOrSF_JLE234 = or i1 %CmpZF_JLE232, %CmpOF_JLE233
  br i1 %ZFOrSF_JLE234, label %bb.15, label %bb.14

bb.14:                                            ; preds = %bb.13
  store i32 32767, i32* %RBP_N.72, align 1
  br label %bb.18

bb.15:                                            ; preds = %bb.13
  %82 = load i32, i32* %RBP_N.72, align 1
  %83 = zext i32 %82 to i64
  %84 = zext i32 -32768 to i64
  %85 = sub i64 %83, %84
  %86 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %83, i64 %84)
  %CF131 = extractvalue { i64, i1 } %86, 1
  %ZF132 = icmp eq i64 %85, 0
  %highbit133 = and i64 -9223372036854775808, %85
  %SF134 = icmp ne i64 %highbit133, 0
  %87 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %83, i64 %84)
  %OF135 = extractvalue { i64, i1 } %87, 1
  %CmpSFOF_JGE = icmp eq i1 %SF134, %OF135
  br i1 %CmpSFOF_JGE, label %bb.17, label %bb.16

bb.16:                                            ; preds = %bb.15
  store i32 -32768, i32* %RBP_N.72, align 1
  br label %bb.17

bb.17:                                            ; preds = %bb.16, %bb.15
  br label %bb.18

bb.18:                                            ; preds = %bb.17, %bb.14
  %memload136 = load i32, i32* %RBP_N.56, align 1
  %memload137 = load i32, i32* %RBP_N.60, align 4
  %EAX138 = or i32 %memload136, %memload137
  store i32 %EAX138, i32* %RBP_N.60, align 1
  %88 = bitcast i32* %RBP_N.60 to i64*
  %memload139 = load i64, i64* %88, align 1
  %89 = trunc i64 %memload139 to i32
  %RAX140 = sext i32 %89 to i64
  %memref-idxreg141 = mul i64 4, %RAX140
  %90 = getelementptr inbounds [16 x i32], [16 x i32]* @indexTable, i32 0, i32 0
  %91 = bitcast [16 x i32]* @indexTable to [64 x i8]*
  %92 = getelementptr inbounds [64 x i8], [64 x i8]* %91, i32 0, i32 0
  %93 = ptrtoint i8* %92 to i64
  %memref-disp142 = add i64 %memref-idxreg141, %93
  %94 = inttoptr i64 %memref-disp142 to i32*
  %memload143 = load i32, i32* %94, align 1
  %memload144 = load i32, i32* %RBP_N.80, align 4
  %EAX145 = add i32 %memload143, %memload144
  store i32 %EAX145, i32* %RBP_N.80, align 1
  %95 = load i32, i32* %RBP_N.80, align 1
  %96 = zext i32 %95 to i64
  %97 = zext i32 0 to i64
  %98 = sub i64 %96, %97
  %99 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %96, i64 %97)
  %CF146 = extractvalue { i64, i1 } %99, 1
  %ZF147 = icmp eq i64 %98, 0
  %highbit148 = and i64 -9223372036854775808, %98
  %SF149 = icmp ne i64 %highbit148, 0
  %100 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %96, i64 %97)
  %OF150 = extractvalue { i64, i1 } %100, 1
  %CmpSFOF_JGE235 = icmp eq i1 %SF149, %OF150
  br i1 %CmpSFOF_JGE235, label %bb.20, label %bb.19

bb.19:                                            ; preds = %bb.18
  store i32 0, i32* %RBP_N.80, align 1
  br label %bb.20

bb.20:                                            ; preds = %bb.19, %bb.18
  %101 = load i32, i32* %RBP_N.80, align 1
  %102 = zext i32 %101 to i64
  %103 = zext i32 88 to i64
  %104 = sub i64 %102, %103
  %105 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %102, i64 %103)
  %CF151 = extractvalue { i64, i1 } %105, 1
  %ZF152 = icmp eq i64 %104, 0
  %highbit153 = and i64 -9223372036854775808, %104
  %SF154 = icmp ne i64 %highbit153, 0
  %106 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %102, i64 %103)
  %OF155 = extractvalue { i64, i1 } %106, 1
  %CmpZF_JLE236 = icmp eq i1 %ZF152, true
  %CmpOF_JLE237 = icmp ne i1 %SF154, %OF155
  %ZFOrSF_JLE238 = or i1 %CmpZF_JLE236, %CmpOF_JLE237
  br i1 %ZFOrSF_JLE238, label %bb.22, label %bb.21

bb.21:                                            ; preds = %bb.20
  store i32 88, i32* %RBP_N.80, align 1
  br label %bb.22

bb.22:                                            ; preds = %bb.21, %bb.20
  %107 = bitcast i32* %RBP_N.80 to i64*
  %memload156 = load i64, i64* %107, align 1
  %108 = trunc i64 %memload156 to i32
  %RAX157 = sext i32 %108 to i64
  %memref-idxreg158 = mul i64 4, %RAX157
  %109 = getelementptr inbounds [89 x i32], [89 x i32]* @stepsizeTable, i32 0, i32 0
  %110 = bitcast [89 x i32]* @stepsizeTable to [356 x i8]*
  %111 = getelementptr inbounds [356 x i8], [356 x i8]* %110, i32 0, i32 0
  %112 = ptrtoint i8* %111 to i64
  %memref-disp159 = add i64 %memref-idxreg158, %112
  %113 = inttoptr i64 %memref-disp159 to i32*
  %memload160 = load i32, i32* %113, align 1
  store i32 %memload160, i32* %RBP_N.68, align 1
  %114 = load i32, i32* %RBP_N.88, align 1
  %115 = zext i32 %114 to i64
  %116 = zext i32 0 to i64
  %117 = sub i64 %115, %116
  %118 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %115, i64 %116)
  %CF161 = extractvalue { i64, i1 } %118, 1
  %ZF162 = icmp eq i64 %117, 0
  %highbit163 = and i64 -9223372036854775808, %117
  %SF164 = icmp ne i64 %highbit163, 0
  %119 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %115, i64 %116)
  %OF165 = extractvalue { i64, i1 } %119, 1
  %CmpZF_JE239 = icmp eq i1 %ZF162, true
  br i1 %CmpZF_JE239, label %bb.24, label %bb.23

bb.23:                                            ; preds = %bb.22
  %memload166 = load i32, i32* %RBP_N.60, align 1
  %EAX170 = shl i32 %memload166, 4
  %ZF167 = icmp eq i32 %EAX170, 0
  %highbit168 = and i32 -2147483648, %EAX170
  %SF169 = icmp ne i32 %highbit168, 0
  %EAX174 = and i32 %EAX170, 240
  %ZF171 = icmp eq i32 %EAX174, 0
  %highbit172 = and i32 -2147483648, %EAX174
  %SF173 = icmp ne i32 %highbit172, 0
  store i32 %EAX174, i32* %RBP_N.84, align 1
  br label %bb.25

bb.24:                                            ; preds = %bb.22
  %memload175 = load i32, i32* %RBP_N.60, align 1
  %EAX179 = and i32 %memload175, 15
  %ZF176 = icmp eq i32 %EAX179, 0
  %highbit177 = and i32 -2147483648, %EAX179
  %SF178 = icmp ne i32 %highbit177, 0
  %memload180 = load i32, i32* %RBP_N.84, align 4
  %EAX181 = or i32 %EAX179, %memload180
  %CL = trunc i32 %EAX181 to i8
  %memload182 = load i64, i64* %RBP_N.48, align 1
  %RDX = add i64 %memload182, 1
  %120 = call { i64, i1 } @llvm.uadd.with.overflow.i64(i64 %memload182, i64 1)
  %CF183 = extractvalue { i64, i1 } %120, 1
  %ZF184 = icmp eq i64 %RDX, 0
  %highbit185 = and i64 -9223372036854775808, %RDX
  %SF186 = icmp ne i64 %highbit185, 0
  %121 = call { i64, i1 } @llvm.sadd.with.overflow.i64(i64 %memload182, i64 1)
  %OF187 = extractvalue { i64, i1 } %121, 1
  store i64 %RDX, i64* %RBP_N.48, align 1
  %122 = inttoptr i64 %memload182 to i8*
  store i8 %CL, i8* %122, align 1
  br label %bb.25

bb.25:                                            ; preds = %bb.24, %bb.23
  %123 = load i32, i32* %RBP_N.88, align 1
  %124 = zext i32 %123 to i64
  %125 = zext i32 0 to i64
  %126 = sub i64 %124, %125
  %127 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %124, i64 %125)
  %CF188 = extractvalue { i64, i1 } %127, 1
  %ZF189 = icmp eq i64 %126, 0
  %highbit190 = and i64 -9223372036854775808, %126
  %SF191 = icmp ne i64 %highbit190, 0
  %128 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %124, i64 %125)
  %OF192 = extractvalue { i64, i1 } %128, 1
  %AL = icmp eq i1 %ZF189, false
  %129 = zext i1 %AL to i8
  %AL196 = xor i8 %129, -1
  %ZF193 = icmp eq i8 %AL196, 0
  %highbit194 = and i8 -128, %AL196
  %SF195 = icmp ne i8 %highbit194, 0
  %AL200 = and i8 %AL196, 1
  %ZF197 = icmp eq i8 %AL200, 0
  %highbit198 = and i8 -128, %AL200
  %SF199 = icmp ne i8 %highbit198, 0
  %EAX201 = zext i8 %AL200 to i32
  store i32 %EAX201, i32* %RBP_N.88, align 1
  %memload202 = load i32, i32* %RBP_N.20, align 1
  %EAX208 = add i32 %memload202, -1
  %130 = call { i32, i1 } @llvm.uadd.with.overflow.i32(i32 %memload202, i32 -1)
  %CF203 = extractvalue { i32, i1 } %130, 1
  %ZF204 = icmp eq i32 %EAX208, 0
  %highbit205 = and i32 -2147483648, %EAX208
  %SF206 = icmp ne i32 %highbit205, 0
  %131 = call { i32, i1 } @llvm.sadd.with.overflow.i32(i32 %memload202, i32 -1)
  %OF207 = extractvalue { i32, i1 } %131, 1
  store i32 %EAX208, i32* %RBP_N.20, align 1
  br label %bb.1

bb.26:                                            ; preds = %bb.1
  %132 = load i32, i32* %RBP_N.88, align 1
  %133 = zext i32 %132 to i64
  %134 = zext i32 0 to i64
  %135 = sub i64 %133, %134
  %136 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %133, i64 %134)
  %CF209 = extractvalue { i64, i1 } %136, 1
  %ZF210 = icmp eq i64 %135, 0
  %highbit211 = and i64 -9223372036854775808, %135
  %SF212 = icmp ne i64 %highbit211, 0
  %137 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %133, i64 %134)
  %OF213 = extractvalue { i64, i1 } %137, 1
  %CmpZF_JNE = icmp eq i1 %ZF210, false
  br i1 %CmpZF_JNE, label %bb.28, label %bb.27

bb.27:                                            ; preds = %bb.26
  %memload214 = load i32, i32* %RBP_N.84, align 1
  %CL215 = trunc i32 %memload214 to i8
  %memload216 = load i64, i64* %RBP_N.48, align 1
  %RDX222 = add i64 %memload216, 1
  %138 = call { i64, i1 } @llvm.uadd.with.overflow.i64(i64 %memload216, i64 1)
  %CF217 = extractvalue { i64, i1 } %138, 1
  %ZF218 = icmp eq i64 %RDX222, 0
  %highbit219 = and i64 -9223372036854775808, %RDX222
  %SF220 = icmp ne i64 %highbit219, 0
  %139 = call { i64, i1 } @llvm.sadd.with.overflow.i64(i64 %memload216, i64 1)
  %OF221 = extractvalue { i64, i1 } %139, 1
  store i64 %RDX222, i64* %RBP_N.48, align 1
  %140 = inttoptr i64 %memload216 to i8*
  store i8 %CL215, i8* %140, align 1
  br label %bb.28

bb.28:                                            ; preds = %bb.27, %bb.26
  %memload223 = load i32, i32* %RBP_N.72, align 1
  %CX = trunc i32 %memload223 to i16
  %memload224 = load i64, i64* %RBP_N.32, align 1
  %141 = inttoptr i64 %memload224 to i16*
  store i16 %CX, i16* %141, align 1
  %memload225 = load i32, i32* %RBP_N.80, align 1
  %CL226 = trunc i32 %memload225 to i8
  %memload227 = load i64, i64* %RBP_N.32, align 1
  %memref-disp228 = add i64 %memload227, 2
  %142 = inttoptr i64 %memref-disp228 to i8*
  store i8 %CL226, i8* %142, align 1
  ret i64 %memload227
}

define dso_local i64 @adpcm_decoder(i64 %arg1, i64 %arg2, i32 %arg3, i64 %arg4) {
entry:
  %stktop_4 = alloca i8, i32 88, align 1
  %RBP_N.80 = bitcast i8* %stktop_4 to i32*
  %tos = ptrtoint i8* %stktop_4 to i64
  %0 = add i64 %tos, 12
  %RBP_N.76 = inttoptr i64 %0 to i32*
  %1 = add i64 %tos, 16
  %RBP_N.72 = inttoptr i64 %1 to i32*
  %2 = add i64 %tos, 20
  %RBP_N.68 = inttoptr i64 %2 to i32*
  %3 = add i64 %tos, 24
  %RBP_N.64 = inttoptr i64 %3 to i32*
  %4 = add i64 %tos, 28
  %RBP_N.60 = inttoptr i64 %4 to i32*
  %5 = add i64 %tos, 32
  %RBP_N.56 = inttoptr i64 %5 to i32*
  %6 = add i64 %tos, 36
  %RBP_N.52 = inttoptr i64 %6 to i32*
  %7 = add i64 %tos, 40
  %RBP_N.48 = inttoptr i64 %7 to i64*
  %8 = add i64 %tos, 48
  %RBP_N.40 = inttoptr i64 %8 to i64*
  %9 = add i64 %tos, 56
  %RBP_N.32 = inttoptr i64 %9 to i64*
  %10 = add i64 %tos, 68
  %RBP_N.20 = inttoptr i64 %10 to i32*
  %11 = add i64 %tos, 72
  %RBP_N.16 = inttoptr i64 %11 to i64*
  %12 = add i64 %tos, 80
  %RBP_N.8 = inttoptr i64 %12 to i64*
  %13 = add i64 %tos, 0
  %RSP_P.0 = inttoptr i64 %13 to i64*
  %RBP = ptrtoint i64* %RSP_P.0 to i64
  store i64 %arg1, i64* %RBP_N.8, align 1
  store i64 %arg2, i64* %RBP_N.16, align 1
  store i32 %arg3, i32* %RBP_N.20, align 1
  store i64 %arg4, i64* %RBP_N.32, align 1
  store i32 0, i32* %RBP_N.76, align 1
  %memload = load i64, i64* %RBP_N.16, align 1
  store i64 %memload, i64* %RBP_N.48, align 1
  %memload1 = load i64, i64* %RBP_N.8, align 1
  store i64 %memload1, i64* %RBP_N.40, align 1
  %memload2 = load i64, i64* %RBP_N.32, align 1
  %14 = inttoptr i64 %memload2 to i32*
  %memload3 = load i32, i32* %14, align 1
  %15 = trunc i32 %memload3 to i16
  %EAX = sext i16 %15 to i32
  store i32 %EAX, i32* %RBP_N.64, align 1
  %memload4 = load i64, i64* %RBP_N.32, align 1
  %memref-disp = add i64 %memload4, 2
  %16 = inttoptr i64 %memref-disp to i32*
  %memload5 = load i32, i32* %16, align 1
  %17 = trunc i32 %memload5 to i8
  %EAX6 = sext i8 %17 to i32
  store i32 %EAX6, i32* %RBP_N.72, align 1
  %18 = bitcast i32* %RBP_N.72 to i64*
  %memload7 = load i64, i64* %18, align 1
  %19 = trunc i64 %memload7 to i32
  %RAX = sext i32 %19 to i64
  %memref-idxreg = mul i64 4, %RAX
  %20 = getelementptr inbounds [89 x i32], [89 x i32]* @stepsizeTable, i32 0, i32 0
  %21 = bitcast [89 x i32]* @stepsizeTable to [356 x i8]*
  %22 = getelementptr inbounds [356 x i8], [356 x i8]* %21, i32 0, i32 0
  %23 = ptrtoint i8* %22 to i64
  %memref-disp8 = add i64 %memref-idxreg, %23
  %24 = inttoptr i64 %memref-disp8 to i32*
  %memload9 = load i32, i32* %24, align 1
  store i32 %memload9, i32* %RBP_N.60, align 1
  store i32 0, i32* %RBP_N.80, align 1
  br label %bb.1

bb.1:                                             ; preds = %entry, %bb.23
  %25 = load i32, i32* %RBP_N.20, align 1
  %26 = zext i32 %25 to i64
  %27 = zext i32 0 to i64
  %28 = sub i64 %26, %27
  %29 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %26, i64 %27)
  %CF = extractvalue { i64, i1 } %29, 1
  %ZF = icmp eq i64 %28, 0
  %highbit = and i64 -9223372036854775808, %28
  %SF = icmp ne i64 %highbit, 0
  %30 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %26, i64 %27)
  %OF = extractvalue { i64, i1 } %30, 1
  %CmpZF_JLE = icmp eq i1 %ZF, true
  %CmpOF_JLE = icmp ne i1 %SF, %OF
  %ZFOrSF_JLE = or i1 %CmpZF_JLE, %CmpOF_JLE
  br i1 %ZFOrSF_JLE, label %bb.24, label %bb.2

bb.2:                                             ; preds = %bb.1
  %31 = load i32, i32* %RBP_N.80, align 1
  %32 = zext i32 %31 to i64
  %33 = zext i32 0 to i64
  %34 = sub i64 %32, %33
  %35 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %32, i64 %33)
  %CF10 = extractvalue { i64, i1 } %35, 1
  %ZF11 = icmp eq i64 %34, 0
  %highbit12 = and i64 -9223372036854775808, %34
  %SF13 = icmp ne i64 %highbit12, 0
  %36 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %32, i64 %33)
  %OF14 = extractvalue { i64, i1 } %36, 1
  %CmpZF_JE = icmp eq i1 %ZF11, true
  br i1 %CmpZF_JE, label %bb.4, label %bb.3

bb.3:                                             ; preds = %bb.2
  %memload15 = load i32, i32* %RBP_N.76, align 1
  %EAX19 = and i32 %memload15, 15
  %ZF16 = icmp eq i32 %EAX19, 0
  %highbit17 = and i32 -2147483648, %EAX19
  %SF18 = icmp ne i32 %highbit17, 0
  store i32 %EAX19, i32* %RBP_N.56, align 1
  br label %bb.5

bb.4:                                             ; preds = %bb.2
  %memload20 = load i64, i64* %RBP_N.40, align 1
  %RCX = add i64 %memload20, 1
  %37 = call { i64, i1 } @llvm.uadd.with.overflow.i64(i64 %memload20, i64 1)
  %CF21 = extractvalue { i64, i1 } %37, 1
  %ZF22 = icmp eq i64 %RCX, 0
  %highbit23 = and i64 -9223372036854775808, %RCX
  %SF24 = icmp ne i64 %highbit23, 0
  %38 = call { i64, i1 } @llvm.sadd.with.overflow.i64(i64 %memload20, i64 1)
  %OF25 = extractvalue { i64, i1 } %38, 1
  store i64 %RCX, i64* %RBP_N.40, align 1
  %39 = inttoptr i64 %memload20 to i32*
  %memload26 = load i32, i32* %39, align 1
  %40 = trunc i32 %memload26 to i8
  %EAX27 = sext i8 %40 to i32
  store i32 %EAX27, i32* %RBP_N.76, align 1
  %memload28 = load i32, i32* %RBP_N.76, align 1
  %EAX32 = lshr i32 %memload28, 4
  %ZF29 = icmp eq i32 %EAX32, 0
  %highbit30 = and i32 -2147483648, %EAX32
  %SF31 = icmp ne i32 %highbit30, 0
  %EAX36 = and i32 %EAX32, 15
  %ZF33 = icmp eq i32 %EAX36, 0
  %highbit34 = and i32 -2147483648, %EAX36
  %SF35 = icmp ne i32 %highbit34, 0
  store i32 %EAX36, i32* %RBP_N.56, align 1
  br label %bb.5

bb.5:                                             ; preds = %bb.4, %bb.3
  %41 = load i32, i32* %RBP_N.80, align 1
  %42 = zext i32 %41 to i64
  %43 = zext i32 0 to i64
  %44 = sub i64 %42, %43
  %45 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %42, i64 %43)
  %CF37 = extractvalue { i64, i1 } %45, 1
  %ZF38 = icmp eq i64 %44, 0
  %highbit39 = and i64 -9223372036854775808, %44
  %SF40 = icmp ne i64 %highbit39, 0
  %46 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %42, i64 %43)
  %OF41 = extractvalue { i64, i1 } %46, 1
  %AL = icmp eq i1 %ZF38, false
  %47 = zext i1 %AL to i8
  %AL45 = xor i8 %47, -1
  %ZF42 = icmp eq i8 %AL45, 0
  %highbit43 = and i8 -128, %AL45
  %SF44 = icmp ne i8 %highbit43, 0
  %AL49 = and i8 %AL45, 1
  %ZF46 = icmp eq i8 %AL49, 0
  %highbit47 = and i8 -128, %AL49
  %SF48 = icmp ne i8 %highbit47, 0
  %EAX50 = zext i8 %AL49 to i32
  store i32 %EAX50, i32* %RBP_N.80, align 1
  %48 = bitcast i32* %RBP_N.56 to i64*
  %memload51 = load i64, i64* %48, align 1
  %49 = trunc i64 %memload51 to i32
  %RAX52 = sext i32 %49 to i64
  %memref-idxreg53 = mul i64 4, %RAX52
  %50 = getelementptr inbounds [16 x i32], [16 x i32]* @indexTable, i32 0, i32 0
  %51 = bitcast [16 x i32]* @indexTable to [64 x i8]*
  %52 = getelementptr inbounds [64 x i8], [64 x i8]* %51, i32 0, i32 0
  %53 = ptrtoint i8* %52 to i64
  %memref-disp54 = add i64 %memref-idxreg53, %53
  %54 = inttoptr i64 %memref-disp54 to i32*
  %memload55 = load i32, i32* %54, align 1
  %memload56 = load i32, i32* %RBP_N.72, align 4
  %EAX57 = add i32 %memload55, %memload56
  store i32 %EAX57, i32* %RBP_N.72, align 1
  %55 = load i32, i32* %RBP_N.72, align 1
  %56 = zext i32 %55 to i64
  %57 = zext i32 0 to i64
  %58 = sub i64 %56, %57
  %59 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %56, i64 %57)
  %CF58 = extractvalue { i64, i1 } %59, 1
  %ZF59 = icmp eq i64 %58, 0
  %highbit60 = and i64 -9223372036854775808, %58
  %SF61 = icmp ne i64 %highbit60, 0
  %60 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %56, i64 %57)
  %OF62 = extractvalue { i64, i1 } %60, 1
  %CmpSFOF_JGE = icmp eq i1 %SF61, %OF62
  br i1 %CmpSFOF_JGE, label %bb.7, label %bb.6

bb.6:                                             ; preds = %bb.5
  store i32 0, i32* %RBP_N.72, align 1
  br label %bb.7

bb.7:                                             ; preds = %bb.6, %bb.5
  %61 = load i32, i32* %RBP_N.72, align 1
  %62 = zext i32 %61 to i64
  %63 = zext i32 88 to i64
  %64 = sub i64 %62, %63
  %65 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %62, i64 %63)
  %CF63 = extractvalue { i64, i1 } %65, 1
  %ZF64 = icmp eq i64 %64, 0
  %highbit65 = and i64 -9223372036854775808, %64
  %SF66 = icmp ne i64 %highbit65, 0
  %66 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %62, i64 %63)
  %OF67 = extractvalue { i64, i1 } %66, 1
  %CmpZF_JLE181 = icmp eq i1 %ZF64, true
  %CmpOF_JLE182 = icmp ne i1 %SF66, %OF67
  %ZFOrSF_JLE183 = or i1 %CmpZF_JLE181, %CmpOF_JLE182
  br i1 %ZFOrSF_JLE183, label %bb.9, label %bb.8

bb.8:                                             ; preds = %bb.7
  store i32 88, i32* %RBP_N.72, align 1
  br label %bb.9

bb.9:                                             ; preds = %bb.8, %bb.7
  %memload68 = load i32, i32* %RBP_N.56, align 1
  %EAX72 = and i32 %memload68, 8
  %ZF69 = icmp eq i32 %EAX72, 0
  %highbit70 = and i32 -2147483648, %EAX72
  %SF71 = icmp ne i32 %highbit70, 0
  store i32 %EAX72, i32* %RBP_N.52, align 1
  %memload73 = load i32, i32* %RBP_N.56, align 1
  %EAX77 = and i32 %memload73, 7
  %ZF74 = icmp eq i32 %EAX77, 0
  %highbit75 = and i32 -2147483648, %EAX77
  %SF76 = icmp ne i32 %highbit75, 0
  store i32 %EAX77, i32* %RBP_N.56, align 1
  %memload78 = load i32, i32* %RBP_N.60, align 1
  %EAX82 = lshr i32 %memload78, 3
  %ZF79 = icmp eq i32 %EAX82, 0
  %highbit80 = and i32 -2147483648, %EAX82
  %SF81 = icmp ne i32 %highbit80, 0
  store i32 %EAX82, i32* %RBP_N.68, align 1
  %memload83 = load i32, i32* %RBP_N.56, align 1
  %EAX87 = and i32 %memload83, 4
  %ZF84 = icmp eq i32 %EAX87, 0
  %highbit85 = and i32 -2147483648, %EAX87
  %SF86 = icmp ne i32 %highbit85, 0
  %67 = sub i32 %EAX87, 0
  %68 = call { i32, i1 } @llvm.usub.with.overflow.i32(i32 %EAX87, i32 0)
  %CF88 = extractvalue { i32, i1 } %68, 1
  %ZF89 = icmp eq i32 %67, 0
  %highbit90 = and i32 -2147483648, %67
  %SF91 = icmp ne i32 %highbit90, 0
  %69 = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 %EAX87, i32 0)
  %OF92 = extractvalue { i32, i1 } %69, 1
  %CmpZF_JE184 = icmp eq i1 %ZF89, true
  br i1 %CmpZF_JE184, label %bb.11, label %bb.10

bb.10:                                            ; preds = %bb.9
  %memload93 = load i32, i32* %RBP_N.60, align 1
  %memload94 = load i32, i32* %RBP_N.68, align 4
  %EAX95 = add i32 %memload93, %memload94
  store i32 %EAX95, i32* %RBP_N.68, align 1
  br label %bb.11

bb.11:                                            ; preds = %bb.10, %bb.9
  %memload96 = load i32, i32* %RBP_N.56, align 1
  %EAX100 = and i32 %memload96, 2
  %ZF97 = icmp eq i32 %EAX100, 0
  %highbit98 = and i32 -2147483648, %EAX100
  %SF99 = icmp ne i32 %highbit98, 0
  %70 = sub i32 %EAX100, 0
  %71 = call { i32, i1 } @llvm.usub.with.overflow.i32(i32 %EAX100, i32 0)
  %CF101 = extractvalue { i32, i1 } %71, 1
  %ZF102 = icmp eq i32 %70, 0
  %highbit103 = and i32 -2147483648, %70
  %SF104 = icmp ne i32 %highbit103, 0
  %72 = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 %EAX100, i32 0)
  %OF105 = extractvalue { i32, i1 } %72, 1
  %CmpZF_JE185 = icmp eq i1 %ZF102, true
  br i1 %CmpZF_JE185, label %bb.13, label %bb.12

bb.12:                                            ; preds = %bb.11
  %memload106 = load i32, i32* %RBP_N.60, align 1
  %EAX110 = lshr i32 %memload106, 1
  %ZF107 = icmp eq i32 %EAX110, 0
  %highbit108 = and i32 -2147483648, %EAX110
  %SF109 = icmp ne i32 %highbit108, 0
  %memload111 = load i32, i32* %RBP_N.68, align 4
  %EAX112 = add i32 %EAX110, %memload111
  store i32 %EAX112, i32* %RBP_N.68, align 1
  br label %bb.13

bb.13:                                            ; preds = %bb.12, %bb.11
  %memload113 = load i32, i32* %RBP_N.56, align 1
  %EAX117 = and i32 %memload113, 1
  %ZF114 = icmp eq i32 %EAX117, 0
  %highbit115 = and i32 -2147483648, %EAX117
  %SF116 = icmp ne i32 %highbit115, 0
  %73 = sub i32 %EAX117, 0
  %74 = call { i32, i1 } @llvm.usub.with.overflow.i32(i32 %EAX117, i32 0)
  %CF118 = extractvalue { i32, i1 } %74, 1
  %ZF119 = icmp eq i32 %73, 0
  %highbit120 = and i32 -2147483648, %73
  %SF121 = icmp ne i32 %highbit120, 0
  %75 = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 %EAX117, i32 0)
  %OF122 = extractvalue { i32, i1 } %75, 1
  %CmpZF_JE186 = icmp eq i1 %ZF119, true
  br i1 %CmpZF_JE186, label %bb.15, label %bb.14

bb.14:                                            ; preds = %bb.13
  %memload123 = load i32, i32* %RBP_N.60, align 1
  %EAX127 = lshr i32 %memload123, 2
  %ZF124 = icmp eq i32 %EAX127, 0
  %highbit125 = and i32 -2147483648, %EAX127
  %SF126 = icmp ne i32 %highbit125, 0
  %memload128 = load i32, i32* %RBP_N.68, align 4
  %EAX129 = add i32 %EAX127, %memload128
  store i32 %EAX129, i32* %RBP_N.68, align 1
  br label %bb.15

bb.15:                                            ; preds = %bb.14, %bb.13
  %76 = load i32, i32* %RBP_N.52, align 1
  %77 = zext i32 %76 to i64
  %78 = zext i32 0 to i64
  %79 = sub i64 %77, %78
  %80 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %77, i64 %78)
  %CF130 = extractvalue { i64, i1 } %80, 1
  %ZF131 = icmp eq i64 %79, 0
  %highbit132 = and i64 -9223372036854775808, %79
  %SF133 = icmp ne i64 %highbit132, 0
  %81 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %77, i64 %78)
  %OF134 = extractvalue { i64, i1 } %81, 1
  %CmpZF_JE187 = icmp eq i1 %ZF131, true
  br i1 %CmpZF_JE187, label %bb.17, label %bb.16

bb.16:                                            ; preds = %bb.15
  %memload135 = load i32, i32* %RBP_N.68, align 1
  %memload136 = load i32, i32* %RBP_N.64, align 1
  %EAX137 = sub i32 %memload136, %memload135
  %82 = call { i32, i1 } @llvm.usub.with.overflow.i32(i32 %memload136, i32 %memload135)
  %CF138 = extractvalue { i32, i1 } %82, 1
  %ZF139 = icmp eq i32 %EAX137, 0
  %highbit140 = and i32 -2147483648, %EAX137
  %SF141 = icmp ne i32 %highbit140, 0
  %83 = call { i32, i1 } @llvm.ssub.with.overflow.i32(i32 %memload136, i32 %memload135)
  %OF142 = extractvalue { i32, i1 } %83, 1
  store i32 %EAX137, i32* %RBP_N.64, align 1
  br label %bb.18

bb.17:                                            ; preds = %bb.15
  %memload143 = load i32, i32* %RBP_N.68, align 1
  %memload144 = load i32, i32* %RBP_N.64, align 4
  %EAX145 = add i32 %memload143, %memload144
  store i32 %EAX145, i32* %RBP_N.64, align 1
  br label %bb.18

bb.18:                                            ; preds = %bb.17, %bb.16
  %84 = load i32, i32* %RBP_N.64, align 1
  %85 = zext i32 %84 to i64
  %86 = zext i32 32767 to i64
  %87 = sub i64 %85, %86
  %88 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %85, i64 %86)
  %CF146 = extractvalue { i64, i1 } %88, 1
  %ZF147 = icmp eq i64 %87, 0
  %highbit148 = and i64 -9223372036854775808, %87
  %SF149 = icmp ne i64 %highbit148, 0
  %89 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %85, i64 %86)
  %OF150 = extractvalue { i64, i1 } %89, 1
  %CmpZF_JLE188 = icmp eq i1 %ZF147, true
  %CmpOF_JLE189 = icmp ne i1 %SF149, %OF150
  %ZFOrSF_JLE190 = or i1 %CmpZF_JLE188, %CmpOF_JLE189
  br i1 %ZFOrSF_JLE190, label %bb.20, label %bb.19

bb.19:                                            ; preds = %bb.18
  store i32 32767, i32* %RBP_N.64, align 1
  br label %bb.23

bb.20:                                            ; preds = %bb.18
  %90 = load i32, i32* %RBP_N.64, align 1
  %91 = zext i32 %90 to i64
  %92 = zext i32 -32768 to i64
  %93 = sub i64 %91, %92
  %94 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %91, i64 %92)
  %CF151 = extractvalue { i64, i1 } %94, 1
  %ZF152 = icmp eq i64 %93, 0
  %highbit153 = and i64 -9223372036854775808, %93
  %SF154 = icmp ne i64 %highbit153, 0
  %95 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %91, i64 %92)
  %OF155 = extractvalue { i64, i1 } %95, 1
  %CmpSFOF_JGE191 = icmp eq i1 %SF154, %OF155
  br i1 %CmpSFOF_JGE191, label %bb.22, label %bb.21

bb.21:                                            ; preds = %bb.20
  store i32 -32768, i32* %RBP_N.64, align 1
  br label %bb.22

bb.22:                                            ; preds = %bb.21, %bb.20
  br label %bb.23

bb.23:                                            ; preds = %bb.22, %bb.19
  %96 = bitcast i32* %RBP_N.72 to i64*
  %memload156 = load i64, i64* %96, align 1
  %97 = trunc i64 %memload156 to i32
  %RAX157 = sext i32 %97 to i64
  %memref-idxreg158 = mul i64 4, %RAX157
  %98 = getelementptr inbounds [89 x i32], [89 x i32]* @stepsizeTable, i32 0, i32 0
  %99 = bitcast [89 x i32]* @stepsizeTable to [356 x i8]*
  %100 = getelementptr inbounds [356 x i8], [356 x i8]* %99, i32 0, i32 0
  %101 = ptrtoint i8* %100 to i64
  %memref-disp159 = add i64 %memref-idxreg158, %101
  %102 = inttoptr i64 %memref-disp159 to i32*
  %memload160 = load i32, i32* %102, align 1
  store i32 %memload160, i32* %RBP_N.60, align 1
  %memload161 = load i32, i32* %RBP_N.64, align 1
  %CX = trunc i32 %memload161 to i16
  %memload162 = load i64, i64* %RBP_N.48, align 1
  %RDX = add i64 %memload162, 2
  %103 = call { i64, i1 } @llvm.uadd.with.overflow.i64(i64 %memload162, i64 2)
  %CF163 = extractvalue { i64, i1 } %103, 1
  %ZF164 = icmp eq i64 %RDX, 0
  %highbit165 = and i64 -9223372036854775808, %RDX
  %SF166 = icmp ne i64 %highbit165, 0
  %104 = call { i64, i1 } @llvm.sadd.with.overflow.i64(i64 %memload162, i64 2)
  %OF167 = extractvalue { i64, i1 } %104, 1
  store i64 %RDX, i64* %RBP_N.48, align 1
  %105 = inttoptr i64 %memload162 to i16*
  store i16 %CX, i16* %105, align 1
  %memload168 = load i32, i32* %RBP_N.20, align 1
  %EAX174 = add i32 %memload168, -1
  %106 = call { i32, i1 } @llvm.uadd.with.overflow.i32(i32 %memload168, i32 -1)
  %CF169 = extractvalue { i32, i1 } %106, 1
  %ZF170 = icmp eq i32 %EAX174, 0
  %highbit171 = and i32 -2147483648, %EAX174
  %SF172 = icmp ne i32 %highbit171, 0
  %107 = call { i32, i1 } @llvm.sadd.with.overflow.i32(i32 %memload168, i32 -1)
  %OF173 = extractvalue { i32, i1 } %107, 1
  store i32 %EAX174, i32* %RBP_N.20, align 1
  br label %bb.1

bb.24:                                            ; preds = %bb.1
  %memload175 = load i32, i32* %RBP_N.64, align 1
  %CX176 = trunc i32 %memload175 to i16
  %memload177 = load i64, i64* %RBP_N.32, align 1
  %108 = inttoptr i64 %memload177 to i16*
  store i16 %CX176, i16* %108, align 1
  %memload178 = load i32, i32* %RBP_N.72, align 1
  %CL = trunc i32 %memload178 to i8
  %memload179 = load i64, i64* %RBP_N.32, align 1
  %memref-disp180 = add i64 %memload179, 2
  %109 = inttoptr i64 %memref-disp180 to i8*
  store i8 %CL, i8* %109, align 1
  ret i64 %memload179
}

declare dso_local i8* @getenv(i8*)

declare dso_local i32 @atol(i8*)

declare dso_local i32 @read(i32, i8*, i32)

declare dso_local void @perror(i8*)

declare dso_local void @exit(i32)

declare dso_local i32 @write(i32, i8*, i32)

define dso_local i32 @main() {
entry:
  %EDI-SKT-LOC = alloca i64, align 8
  %stktop_8 = alloca i8, i32 56, align 1
  %RSPAdj_N.48 = bitcast i8* %stktop_8 to i64*
  %tos = ptrtoint i8* %stktop_8 to i64
  %0 = add i64 %tos, 16
  %RBP_N.40 = inttoptr i64 %0 to i32*
  %1 = add i64 %tos, 24
  %RBP_N.32 = inttoptr i64 %1 to i32*
  %2 = add i64 %tos, 28
  %RBP_N.28 = inttoptr i64 %2 to i32*
  %3 = add i64 %tos, 32
  %RBP_N.24 = inttoptr i64 %3 to i64*
  %4 = add i64 %tos, 40
  %RBP_N.16 = inttoptr i64 %4 to i64*
  %5 = add i64 %tos, 52
  %RBP_N.4 = inttoptr i64 %5 to i32*
  %6 = add i64 %tos, 0
  %RSP_P.0 = inttoptr i64 %6 to i64*
  %RBP = ptrtoint i64* %RSP_P.0 to i64
  store i32 0, i32* %RBP_N.4, align 1
  %7 = zext i32 0 to i64
  store i64 %7, i64* %RBP_N.16, align 1
  %8 = zext i32 1 to i64
  store i64 %8, i64* %RBP_N.24, align 1
  store i32 0, i32* %RBP_N.28, align 1
  %9 = call i8* @getenv(i8* getelementptr inbounds ([30 x i8], [30 x i8]* @rodata_14, i32 0, i32 4))
  %RAX = ptrtoint i8* %9 to i64
  %10 = sub i64 %RAX, 0
  %11 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %RAX, i64 0)
  %CF = extractvalue { i64, i1 } %11, 1
  %ZF = icmp eq i64 %10, 0
  %highbit = and i64 -9223372036854775808, %10
  %SF = icmp ne i64 %highbit, 0
  %12 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %RAX, i64 0)
  %OF = extractvalue { i64, i1 } %12, 1
  %13 = ptrtoint i8* getelementptr inbounds ([30 x i8], [30 x i8]* @rodata_14, i32 0, i32 4) to i64
  store i64 %13, i64* %EDI-SKT-LOC, align 1
  %CmpZF_JE = icmp eq i1 %ZF, true
  br i1 %CmpZF_JE, label %bb.2, label %bb.1

bb.1:                                             ; preds = %entry
  %14 = call i8* @getenv(i8* getelementptr inbounds ([30 x i8], [30 x i8]* @rodata_14, i32 0, i32 4))
  %RAX1 = ptrtoint i8* %14 to i64
  %15 = inttoptr i64 %RAX1 to i8*
  %EAX = call i32 @atol(i8* %15)
  %16 = zext i32 %EAX to i64
  store i64 %16, i64* %RBP_N.24, align 1
  store i64 %RAX1, i64* %EDI-SKT-LOC, align 1
  br label %bb.2

bb.2:                                             ; preds = %bb.1, %entry
  br label %bb.3

bb.3:                                             ; preds = %bb.10, %bb.2
  %memload = load i32, i32* @state, align 1
  store i32 %memload, i32* %RBP_N.40, align 1
  %17 = load i64, i64* %EDI-SKT-LOC, align 1
  %EDI = trunc i64 %17 to i32
  %18 = bitcast [2000 x i8]* @sbuf to [250 x i64]*
  %RSI = getelementptr inbounds [250 x i64], [250 x i64]* %18, i32 0, i32 0
  %19 = bitcast i64* %RSI to i8*
  %EAX2 = call i32 @read(i32 0, i8* %19, i32 2000)
  store i32 %EAX2, i32* %RBP_N.32, align 1
  %20 = load i32, i32* %RBP_N.32, align 1
  %21 = zext i32 %20 to i64
  %22 = zext i32 0 to i64
  %23 = sub i64 %21, %22
  %24 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %21, i64 %22)
  %CF3 = extractvalue { i64, i1 } %24, 1
  %ZF4 = icmp eq i64 %23, 0
  %highbit5 = and i64 -9223372036854775808, %23
  %SF6 = icmp ne i64 %highbit5, 0
  %25 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %21, i64 %22)
  %OF7 = extractvalue { i64, i1 } %25, 1
  %CmpSFOF_JGE = icmp eq i1 %SF6, %OF7
  br i1 %CmpSFOF_JGE, label %bb.5, label %bb.4

bb.4:                                             ; preds = %bb.3
  call void @perror(i8* getelementptr inbounds ([30 x i8], [30 x i8]* @rodata_14, i32 0, i32 19))
  call void @exit(i32 1)
  unreachable

bb.5:                                             ; preds = %bb.3
  %26 = load i32, i32* %RBP_N.32, align 1
  %27 = zext i32 %26 to i64
  %28 = zext i32 0 to i64
  %29 = sub i64 %27, %28
  %30 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %27, i64 %28)
  %CF8 = extractvalue { i64, i1 } %30, 1
  %ZF9 = icmp eq i64 %29, 0
  %highbit10 = and i64 -9223372036854775808, %29
  %SF11 = icmp ne i64 %highbit10, 0
  %31 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %27, i64 %28)
  %OF12 = extractvalue { i64, i1 } %31, 1
  %CmpZF_JNE = icmp eq i1 %ZF9, false
  br i1 %CmpZF_JNE, label %bb.7, label %bb.6

bb.6:                                             ; preds = %bb.5
  br label %bb.11

bb.11:                                            ; preds = %bb.6
  ret i32 0

bb.7:                                             ; preds = %bb.5
  %32 = zext i32 0 to i64
  store i64 %32, i64* %RBP_N.16, align 1
  br label %bb.8

bb.8:                                             ; preds = %bb.7, %bb.9
  %memload13 = load i64, i64* %RBP_N.16, align 1
  %33 = load i64, i64* %RBP_N.24, align 1
  %34 = sub i64 %memload13, %33
  %35 = call { i64, i1 } @llvm.usub.with.overflow.i64(i64 %memload13, i64 %33)
  %CF14 = extractvalue { i64, i1 } %35, 1
  %ZF15 = icmp eq i64 %34, 0
  %highbit16 = and i64 -9223372036854775808, %34
  %SF17 = icmp ne i64 %highbit16, 0
  %36 = call { i64, i1 } @llvm.ssub.with.overflow.i64(i64 %memload13, i64 %33)
  %OF18 = extractvalue { i64, i1 } %36, 1
  %CmpSFOF_JGE38 = icmp eq i1 %SF17, %OF18
  br i1 %CmpSFOF_JGE38, label %bb.10, label %bb.9

bb.9:                                             ; preds = %bb.8
  %memload19 = load i32, i32* %RBP_N.40, align 1
  store i32 %memload19, i32* @state, align 1
  %memload20 = load i32, i32* %RBP_N.32, align 1
  %37 = sext i32 %memload20 to i64
  %38 = lshr i64 %37, 32
  %EDX = trunc i64 %38 to i32
  %39 = sext i32 %memload20 to i64
  %40 = sext i32 %EDX to i64
  %41 = shl nuw i64 %40, 32
  %42 = or i64 %41, %39
  %43 = sext i32 2 to i64
  %44 = sdiv i64 %42, %43
  %EAX21 = trunc i64 %44 to i32
  %45 = srem i64 %42, %43
  %EDX22 = trunc i64 %45 to i32
  %46 = bitcast [2000 x i8]* @sbuf to [250 x i64]*
  %RDI = getelementptr inbounds [250 x i64], [250 x i64]* %46, i32 0, i32 0
  %47 = bitcast [500 x i8]* @abuf to [62 x i64]*
  %RSI23 = getelementptr inbounds [62 x i64], [62 x i64]* %47, i32 0, i32 0
  %48 = ptrtoint i64* %RDI to i64
  %49 = ptrtoint i64* %RSI23 to i64
  %50 = ptrtoint i32* @state to i64
  %RAX24 = call i64 @adpcm_coder(i64 %48, i64 %49, i32 %EAX21, i64 %50)
  %memload25 = load i64, i64* %RBP_N.16, align 1
  %RAX31 = add i64 %memload25, 1
  %51 = call { i64, i1 } @llvm.uadd.with.overflow.i64(i64 %memload25, i64 1)
  %CF26 = extractvalue { i64, i1 } %51, 1
  %ZF27 = icmp eq i64 %RAX31, 0
  %highbit28 = and i64 -9223372036854775808, %RAX31
  %SF29 = icmp ne i64 %highbit28, 0
  %52 = call { i64, i1 } @llvm.sadd.with.overflow.i64(i64 %memload25, i64 1)
  %OF30 = extractvalue { i64, i1 } %52, 1
  store i64 %RAX31, i64* %RBP_N.16, align 1
  br label %bb.8

bb.10:                                            ; preds = %bb.8
  %memload32 = load i32, i32* %RBP_N.32, align 1
  %53 = sext i32 %memload32 to i64
  %54 = lshr i64 %53, 32
  %EDX33 = trunc i64 %54 to i32
  %55 = sext i32 %memload32 to i64
  %56 = sext i32 %EDX33 to i64
  %57 = shl nuw i64 %56, 32
  %58 = or i64 %57, %55
  %59 = sext i32 4 to i64
  %60 = sdiv i64 %58, %59
  %EAX34 = trunc i64 %60 to i32
  %61 = srem i64 %58, %59
  %EDX35 = trunc i64 %61 to i32
  %62 = bitcast [500 x i8]* @abuf to [62 x i64]*
  %RSI36 = getelementptr inbounds [62 x i64], [62 x i64]* %62, i32 0, i32 0
  %63 = bitcast i64* %RSI36 to i8*
  %EAX37 = call i32 @write(i32 1, i8* %63, i32 %EAX34)
  %64 = zext i32 1 to i64
  store i64 %64, i64* %EDI-SKT-LOC, align 1
  br label %bb.3
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i64, i1 } @llvm.usub.with.overflow.i64(i64, i64) #0

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i64, i1 } @llvm.ssub.with.overflow.i64(i64, i64) #0

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i64, i1 } @llvm.uadd.with.overflow.i64(i64, i64) #0

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i64, i1 } @llvm.sadd.with.overflow.i64(i64, i64) #0

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i32, i1 } @llvm.usub.with.overflow.i32(i32, i32) #0

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i32, i1 } @llvm.ssub.with.overflow.i32(i32, i32) #0

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i32, i1 } @llvm.uadd.with.overflow.i32(i32, i32) #0

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare { i32, i1 } @llvm.sadd.with.overflow.i32(i32, i32) #0

attributes #0 = { nofree nosync nounwind readnone speculatable willreturn }

!0 = !{i64 4197488}
