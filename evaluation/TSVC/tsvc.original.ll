; ModuleID = 'tsvc.original.c'
source_filename = "tsvc.original.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.args_t = type { %struct.timeval, %struct.timeval, i8* }
%struct.timeval = type { i64, i64 }
%struct.anon = type { i32, i32 }
%struct.anon.0 = type { i32, i32 }
%struct.anon.1 = type { float, float }
%struct.anon.2 = type { i32*, float }
%struct.anon.3 = type { i32*, i32 }
%struct.anon.4 = type { i32*, i32, i32 }
%struct.anon.5 = type { i32, i32 }
%struct.anon.6 = type { i32, i32 }
%struct.anon.7 = type { i32 }
%struct.anon.8 = type { float, float }
%struct.anon.9 = type { i32*, float }
%struct.anon.10 = type { i32*, i32 }
%struct.anon.11 = type { i32*, i32, i32 }

@__func__.s000 = private unnamed_addr constant [5 x i8] c"s000\00", align 1
@b = dso_local global [32000 x float] zeroinitializer, align 64
@a = dso_local global [32000 x float] zeroinitializer, align 64
@c = dso_local global [32000 x float] zeroinitializer, align 64
@d = dso_local global [32000 x float] zeroinitializer, align 64
@e = dso_local global [32000 x float] zeroinitializer, align 64
@aa = dso_local global [256 x [256 x float]] zeroinitializer, align 64
@bb = dso_local global [256 x [256 x float]] zeroinitializer, align 64
@cc = dso_local global [256 x [256 x float]] zeroinitializer, align 64
@__func__.s111 = private unnamed_addr constant [5 x i8] c"s111\00", align 1
@__func__.s1111 = private unnamed_addr constant [6 x i8] c"s1111\00", align 1
@__func__.s112 = private unnamed_addr constant [5 x i8] c"s112\00", align 1
@__func__.s1112 = private unnamed_addr constant [6 x i8] c"s1112\00", align 1
@__func__.s113 = private unnamed_addr constant [5 x i8] c"s113\00", align 1
@__func__.s1113 = private unnamed_addr constant [6 x i8] c"s1113\00", align 1
@__func__.s114 = private unnamed_addr constant [5 x i8] c"s114\00", align 1
@__func__.s115 = private unnamed_addr constant [5 x i8] c"s115\00", align 1
@__func__.s1115 = private unnamed_addr constant [6 x i8] c"s1115\00", align 1
@__func__.s116 = private unnamed_addr constant [5 x i8] c"s116\00", align 1
@__func__.s118 = private unnamed_addr constant [5 x i8] c"s118\00", align 1
@__func__.s119 = private unnamed_addr constant [5 x i8] c"s119\00", align 1
@__func__.s1119 = private unnamed_addr constant [6 x i8] c"s1119\00", align 1
@__func__.s121 = private unnamed_addr constant [5 x i8] c"s121\00", align 1
@__func__.s122 = private unnamed_addr constant [5 x i8] c"s122\00", align 1
@__func__.s123 = private unnamed_addr constant [5 x i8] c"s123\00", align 1
@__func__.s124 = private unnamed_addr constant [5 x i8] c"s124\00", align 1
@__func__.s125 = private unnamed_addr constant [5 x i8] c"s125\00", align 1
@flat_2d_array = dso_local global [65536 x float] zeroinitializer, align 64
@__func__.s126 = private unnamed_addr constant [5 x i8] c"s126\00", align 1
@__func__.s127 = private unnamed_addr constant [5 x i8] c"s127\00", align 1
@__func__.s128 = private unnamed_addr constant [5 x i8] c"s128\00", align 1
@__func__.s131 = private unnamed_addr constant [5 x i8] c"s131\00", align 1
@__func__.s132 = private unnamed_addr constant [5 x i8] c"s132\00", align 1
@__func__.s141 = private unnamed_addr constant [5 x i8] c"s141\00", align 1
@__func__.s151 = private unnamed_addr constant [5 x i8] c"s151\00", align 1
@__func__.s152 = private unnamed_addr constant [5 x i8] c"s152\00", align 1
@__func__.s161 = private unnamed_addr constant [5 x i8] c"s161\00", align 1
@__func__.s1161 = private unnamed_addr constant [6 x i8] c"s1161\00", align 1
@__func__.s162 = private unnamed_addr constant [5 x i8] c"s162\00", align 1
@__func__.s171 = private unnamed_addr constant [5 x i8] c"s171\00", align 1
@__func__.s172 = private unnamed_addr constant [5 x i8] c"s172\00", align 1
@__func__.s173 = private unnamed_addr constant [5 x i8] c"s173\00", align 1
@__func__.s174 = private unnamed_addr constant [5 x i8] c"s174\00", align 1
@__func__.s175 = private unnamed_addr constant [5 x i8] c"s175\00", align 1
@__func__.s176 = private unnamed_addr constant [5 x i8] c"s176\00", align 1
@__func__.s211 = private unnamed_addr constant [5 x i8] c"s211\00", align 1
@__func__.s212 = private unnamed_addr constant [5 x i8] c"s212\00", align 1
@__func__.s1213 = private unnamed_addr constant [6 x i8] c"s1213\00", align 1
@__func__.s221 = private unnamed_addr constant [5 x i8] c"s221\00", align 1
@__func__.s1221 = private unnamed_addr constant [6 x i8] c"s1221\00", align 1
@__func__.s222 = private unnamed_addr constant [5 x i8] c"s222\00", align 1
@__func__.s231 = private unnamed_addr constant [5 x i8] c"s231\00", align 1
@__func__.s232 = private unnamed_addr constant [5 x i8] c"s232\00", align 1
@__func__.s1232 = private unnamed_addr constant [6 x i8] c"s1232\00", align 1
@__func__.s233 = private unnamed_addr constant [5 x i8] c"s233\00", align 1
@__func__.s2233 = private unnamed_addr constant [6 x i8] c"s2233\00", align 1
@__func__.s235 = private unnamed_addr constant [5 x i8] c"s235\00", align 1
@__func__.s241 = private unnamed_addr constant [5 x i8] c"s241\00", align 1
@__func__.s242 = private unnamed_addr constant [5 x i8] c"s242\00", align 1
@__func__.s243 = private unnamed_addr constant [5 x i8] c"s243\00", align 1
@__func__.s244 = private unnamed_addr constant [5 x i8] c"s244\00", align 1
@__func__.s1244 = private unnamed_addr constant [6 x i8] c"s1244\00", align 1
@__func__.s2244 = private unnamed_addr constant [6 x i8] c"s2244\00", align 1
@__func__.s251 = private unnamed_addr constant [5 x i8] c"s251\00", align 1
@__func__.s1251 = private unnamed_addr constant [6 x i8] c"s1251\00", align 1
@__func__.s2251 = private unnamed_addr constant [6 x i8] c"s2251\00", align 1
@__func__.s3251 = private unnamed_addr constant [6 x i8] c"s3251\00", align 1
@__func__.s252 = private unnamed_addr constant [5 x i8] c"s252\00", align 1
@__func__.s253 = private unnamed_addr constant [5 x i8] c"s253\00", align 1
@__func__.s254 = private unnamed_addr constant [5 x i8] c"s254\00", align 1
@__func__.s255 = private unnamed_addr constant [5 x i8] c"s255\00", align 1
@__func__.s256 = private unnamed_addr constant [5 x i8] c"s256\00", align 1
@__func__.s257 = private unnamed_addr constant [5 x i8] c"s257\00", align 1
@__func__.s258 = private unnamed_addr constant [5 x i8] c"s258\00", align 1
@__func__.s261 = private unnamed_addr constant [5 x i8] c"s261\00", align 1
@__func__.s271 = private unnamed_addr constant [5 x i8] c"s271\00", align 1
@__func__.s272 = private unnamed_addr constant [5 x i8] c"s272\00", align 1
@__func__.s273 = private unnamed_addr constant [5 x i8] c"s273\00", align 1
@__func__.s274 = private unnamed_addr constant [5 x i8] c"s274\00", align 1
@__func__.s275 = private unnamed_addr constant [5 x i8] c"s275\00", align 1
@__func__.s2275 = private unnamed_addr constant [6 x i8] c"s2275\00", align 1
@__func__.s276 = private unnamed_addr constant [5 x i8] c"s276\00", align 1
@__func__.s277 = private unnamed_addr constant [5 x i8] c"s277\00", align 1
@__func__.s278 = private unnamed_addr constant [5 x i8] c"s278\00", align 1
@__func__.s279 = private unnamed_addr constant [5 x i8] c"s279\00", align 1
@__func__.s1279 = private unnamed_addr constant [6 x i8] c"s1279\00", align 1
@__func__.s2710 = private unnamed_addr constant [6 x i8] c"s2710\00", align 1
@__func__.s2711 = private unnamed_addr constant [6 x i8] c"s2711\00", align 1
@__func__.s2712 = private unnamed_addr constant [6 x i8] c"s2712\00", align 1
@__func__.s281 = private unnamed_addr constant [5 x i8] c"s281\00", align 1
@__func__.s1281 = private unnamed_addr constant [6 x i8] c"s1281\00", align 1
@__func__.s291 = private unnamed_addr constant [5 x i8] c"s291\00", align 1
@__func__.s292 = private unnamed_addr constant [5 x i8] c"s292\00", align 1
@__func__.s293 = private unnamed_addr constant [5 x i8] c"s293\00", align 1
@__func__.s2101 = private unnamed_addr constant [6 x i8] c"s2101\00", align 1
@__func__.s2102 = private unnamed_addr constant [6 x i8] c"s2102\00", align 1
@__func__.s2111 = private unnamed_addr constant [6 x i8] c"s2111\00", align 1
@__func__.s311 = private unnamed_addr constant [5 x i8] c"s311\00", align 1
@__func__.s31111 = private unnamed_addr constant [7 x i8] c"s31111\00", align 1
@__func__.s312 = private unnamed_addr constant [5 x i8] c"s312\00", align 1
@__func__.s313 = private unnamed_addr constant [5 x i8] c"s313\00", align 1
@__func__.s314 = private unnamed_addr constant [5 x i8] c"s314\00", align 1
@__func__.s315 = private unnamed_addr constant [5 x i8] c"s315\00", align 1
@__func__.s316 = private unnamed_addr constant [5 x i8] c"s316\00", align 1
@__func__.s317 = private unnamed_addr constant [5 x i8] c"s317\00", align 1
@__func__.s318 = private unnamed_addr constant [5 x i8] c"s318\00", align 1
@__func__.s319 = private unnamed_addr constant [5 x i8] c"s319\00", align 1
@__func__.s3110 = private unnamed_addr constant [6 x i8] c"s3110\00", align 1
@__func__.s13110 = private unnamed_addr constant [7 x i8] c"s13110\00", align 1
@__func__.s3111 = private unnamed_addr constant [6 x i8] c"s3111\00", align 1
@__func__.s3112 = private unnamed_addr constant [6 x i8] c"s3112\00", align 1
@__func__.s3113 = private unnamed_addr constant [6 x i8] c"s3113\00", align 1
@__func__.s321 = private unnamed_addr constant [5 x i8] c"s321\00", align 1
@__func__.s322 = private unnamed_addr constant [5 x i8] c"s322\00", align 1
@__func__.s323 = private unnamed_addr constant [5 x i8] c"s323\00", align 1
@__func__.s331 = private unnamed_addr constant [5 x i8] c"s331\00", align 1
@__func__.s332 = private unnamed_addr constant [5 x i8] c"s332\00", align 1
@__func__.s341 = private unnamed_addr constant [5 x i8] c"s341\00", align 1
@__func__.s342 = private unnamed_addr constant [5 x i8] c"s342\00", align 1
@__func__.s343 = private unnamed_addr constant [5 x i8] c"s343\00", align 1
@__func__.s351 = private unnamed_addr constant [5 x i8] c"s351\00", align 1
@__func__.s1351 = private unnamed_addr constant [6 x i8] c"s1351\00", align 1
@__func__.s352 = private unnamed_addr constant [5 x i8] c"s352\00", align 1
@__func__.s353 = private unnamed_addr constant [5 x i8] c"s353\00", align 1
@__func__.s421 = private unnamed_addr constant [5 x i8] c"s421\00", align 1
@xx = dso_local local_unnamed_addr global float* null, align 8
@yy = dso_local local_unnamed_addr global float* null, align 8
@__func__.s1421 = private unnamed_addr constant [6 x i8] c"s1421\00", align 1
@__func__.s422 = private unnamed_addr constant [5 x i8] c"s422\00", align 1
@__func__.s423 = private unnamed_addr constant [5 x i8] c"s423\00", align 1
@__func__.s424 = private unnamed_addr constant [5 x i8] c"s424\00", align 1
@__func__.s431 = private unnamed_addr constant [5 x i8] c"s431\00", align 1
@__func__.s441 = private unnamed_addr constant [5 x i8] c"s441\00", align 1
@__func__.s442 = private unnamed_addr constant [5 x i8] c"s442\00", align 1
@indx = dso_local local_unnamed_addr global [32000 x i32] zeroinitializer, align 64
@__func__.s443 = private unnamed_addr constant [5 x i8] c"s443\00", align 1
@__func__.s451 = private unnamed_addr constant [5 x i8] c"s451\00", align 1
@__func__.s452 = private unnamed_addr constant [5 x i8] c"s452\00", align 1
@__func__.s453 = private unnamed_addr constant [5 x i8] c"s453\00", align 1
@__func__.s471 = private unnamed_addr constant [5 x i8] c"s471\00", align 1
@x = dso_local local_unnamed_addr global [32000 x float] zeroinitializer, align 64
@__func__.s481 = private unnamed_addr constant [5 x i8] c"s481\00", align 1
@__func__.s482 = private unnamed_addr constant [5 x i8] c"s482\00", align 1
@__func__.s491 = private unnamed_addr constant [5 x i8] c"s491\00", align 1
@__func__.s4112 = private unnamed_addr constant [6 x i8] c"s4112\00", align 1
@__func__.s4113 = private unnamed_addr constant [6 x i8] c"s4113\00", align 1
@__func__.s4114 = private unnamed_addr constant [6 x i8] c"s4114\00", align 1
@__func__.s4115 = private unnamed_addr constant [6 x i8] c"s4115\00", align 1
@__func__.s4116 = private unnamed_addr constant [6 x i8] c"s4116\00", align 1
@__func__.s4117 = private unnamed_addr constant [6 x i8] c"s4117\00", align 1
@__func__.s4121 = private unnamed_addr constant [6 x i8] c"s4121\00", align 1
@__func__.va = private unnamed_addr constant [3 x i8] c"va\00", align 1
@__func__.vag = private unnamed_addr constant [4 x i8] c"vag\00", align 1
@__func__.vas = private unnamed_addr constant [4 x i8] c"vas\00", align 1
@__func__.vif = private unnamed_addr constant [4 x i8] c"vif\00", align 1
@__func__.vpv = private unnamed_addr constant [4 x i8] c"vpv\00", align 1
@__func__.vtv = private unnamed_addr constant [4 x i8] c"vtv\00", align 1
@__func__.vpvtv = private unnamed_addr constant [6 x i8] c"vpvtv\00", align 1
@__func__.vpvts = private unnamed_addr constant [6 x i8] c"vpvts\00", align 1
@__func__.vpvpv = private unnamed_addr constant [6 x i8] c"vpvpv\00", align 1
@__func__.vtvtv = private unnamed_addr constant [6 x i8] c"vtvtv\00", align 1
@__func__.vsumr = private unnamed_addr constant [6 x i8] c"vsumr\00", align 1
@__func__.vdotr = private unnamed_addr constant [6 x i8] c"vdotr\00", align 1
@__func__.vbor = private unnamed_addr constant [5 x i8] c"vbor\00", align 1
@.str = private unnamed_addr constant [11 x i8] c"%10.3f\09%f\0A\00", align 1
@tt = dso_local local_unnamed_addr global [256 x [256 x float]] zeroinitializer, align 64
@str = private unnamed_addr constant [26 x i8] c"Loop \09Time(sec) \09Checksum\00", align 1

; Function Attrs: nounwind optsize uwtable
define dso_local float @s000(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s000, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.022 = phi i32 [ 0, %entry ], [ %inc10, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call12 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call13 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s000, i64 0, i64 0)) #11
  ret float %call13

for.cond.cleanup4:                                ; preds = %for.body5
  %call8 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc10 = add nuw nsw i32 %nl.022, 1
  %exitcond23.not = icmp eq i32 %inc10, 200000
  br i1 %exitcond23.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !2

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %add = fadd float %0, 1.000000e+00
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx7, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !8
}

; Function Attrs: optsize
declare dso_local i32 @initialise_arrays(i8*) local_unnamed_addr #1

; Function Attrs: nofree nounwind optsize
declare dso_local noundef i32 @gettimeofday(%struct.timeval* nocapture noundef, i8* nocapture noundef) local_unnamed_addr #2

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0i8(i64 immarg, i8* nocapture) #3

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0i8(i64 immarg, i8* nocapture) #3

; Function Attrs: optsize
declare dso_local i32 @dummy(float*, float*, float*, float*, float*, [256 x float]*, [256 x float]*, [256 x float]*, float) local_unnamed_addr #1

; Function Attrs: optsize
declare dso_local float @calc_checksum(i8*) local_unnamed_addr #1

; Function Attrs: nounwind optsize uwtable
define dso_local float @s111(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s111, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.025 = phi i32 [ 0, %entry ], [ %inc, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call15 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s111, i64 0, i64 0)) #11
  ret float %call15

for.cond.cleanup4:                                ; preds = %for.body5
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc = add nuw nsw i32 %nl.025, 1
  %exitcond.not = icmp eq i32 %inc, 200000
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !9

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %0 = add nsw i64 %indvars.iv, -1
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %0
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %1, %2
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 2
  %cmp3 = icmp ult i64 %indvars.iv, 31998
  br i1 %cmp3, label %for.body5, label %for.cond.cleanup4, !llvm.loop !10
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s1111(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1111, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.057 = phi i32 [ 0, %entry ], [ %inc36, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call38 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call39 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1111, i64 0, i64 0)) #11
  ret float %call39

for.cond.cleanup4:                                ; preds = %for.body5
  %call34 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc36 = add nuw nsw i32 %nl.057, 1
  %exitcond59.not = icmp eq i32 %inc36, 200000
  br i1 %exitcond59.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !11

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul12 = fmul float %1, %2
  %add = fadd float %mul, %mul12
  %mul17 = fmul float %0, %0
  %add18 = fadd float %mul17, %add
  %add24 = fadd float %mul12, %add18
  %mul29 = fmul float %0, %2
  %add30 = fadd float %mul29, %add24
  %3 = shl nuw nsw i64 %indvars.iv, 1
  %arrayidx33 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %3
  store float %add30, float* %arrayidx33, align 8, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 16000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !12
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s112(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s112, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.026 = phi i32 [ 0, %entry ], [ %inc, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call15 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s112, i64 0, i64 0)) #11
  ret float %call15

for.cond.cleanup4:                                ; preds = %for.body5
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc = add nuw nsw i32 %nl.026, 1
  %exitcond.not = icmp eq i32 %inc, 300000
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !13

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 31998, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %0, %1
  %2 = add nuw nsw i64 %indvars.iv, 1
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %2
  store float %add, float* %arrayidx10, align 4, !tbaa !4
  %indvars.iv.next = add nsw i64 %indvars.iv, -1
  %cmp3.not = icmp eq i64 %indvars.iv, 0
  br i1 %cmp3.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !14
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s1112(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1112, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.022 = phi i32 [ 0, %entry ], [ %inc, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call11 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call12 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1112, i64 0, i64 0)) #11
  ret float %call12

for.cond.cleanup4:                                ; preds = %for.body5
  %call8 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc = add nuw nsw i32 %nl.022, 1
  %exitcond.not = icmp eq i32 %inc, 300000
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !15

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 31999, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %add = fadd float %0, 1.000000e+00
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx7, align 4, !tbaa !4
  %indvars.iv.next = add nsw i64 %indvars.iv, -1
  %cmp3.not = icmp eq i64 %indvars.iv, 0
  br i1 %cmp3.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !16
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s113(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s113, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.022 = phi i32 [ 0, %entry ], [ %inc10, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call12 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call13 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s113, i64 0, i64 0)) #11
  ret float %call13

for.cond.cleanup4:                                ; preds = %for.body5
  %call8 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc10 = add nuw nsw i32 %nl.022, 1
  %exitcond23.not = icmp eq i32 %inc10, 400000
  br i1 %exitcond23.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !17

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %0 = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), align 64, !tbaa !4
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %add = fadd float %0, %1
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx7, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !18
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s1113(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1113, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.022 = phi i32 [ 0, %entry ], [ %inc10, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call12 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call13 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1113, i64 0, i64 0)) #11
  ret float %call13

for.cond.cleanup4:                                ; preds = %for.body5
  %call8 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc10 = add nuw nsw i32 %nl.022, 1
  %exitcond23.not = icmp eq i32 %inc10, 200000
  br i1 %exitcond23.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !19

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %0 = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 16000), align 64, !tbaa !4
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %add = fadd float %0, %1
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx7, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !20
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s114(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s114, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.047 = phi i32 [ 0, %entry ], [ %inc25, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call27 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call28 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s114, i64 0, i64 0)) #11
  ret float %call28

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv48 = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next49, %for.cond.cleanup8 ]
  %cmp743.not = icmp eq i64 %indvars.iv48, 0
  br i1 %cmp743.not, label %for.cond.cleanup8, label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call23 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc25 = add nuw nsw i32 %nl.047, 1
  %exitcond51.not = icmp eq i32 %inc25, 78000
  br i1 %exitcond51.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !21

for.cond.cleanup8:                                ; preds = %for.body9, %for.cond6.preheader
  %indvars.iv.next49 = add nuw nsw i64 %indvars.iv48, 1
  %exitcond50.not = icmp eq i64 %indvars.iv.next49, 256
  br i1 %exitcond50.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !22

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %indvars.iv = phi i64 [ %indvars.iv.next, %for.body9 ], [ 0, %for.cond6.preheader ]
  %arrayidx11 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv, i64 %indvars.iv48
  %0 = load float, float* %arrayidx11, align 4, !tbaa !4
  %arrayidx15 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv48, i64 %indvars.iv
  %1 = load float, float* %arrayidx15, align 4, !tbaa !4
  %add = fadd float %0, %1
  %arrayidx19 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv48, i64 %indvars.iv
  store float %add, float* %arrayidx19, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, %indvars.iv48
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !23
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s115(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s115, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.040 = phi i32 [ 0, %entry ], [ %inc21, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call23 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call24 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s115, i64 0, i64 0)) #11
  ret float %call24

for.cond2.loopexit:                               ; preds = %for.body9, %for.body5
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond45.not = icmp eq i64 %indvars.iv.next44, 256
  br i1 %exitcond45.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !24

for.cond.cleanup4:                                ; preds = %for.cond2.loopexit
  %call19 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc21 = add nuw nsw i32 %nl.040, 1
  %exitcond46.not = icmp eq i32 %inc21, 390000
  br i1 %exitcond46.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !25

for.body5:                                        ; preds = %for.cond2.preheader, %for.cond2.loopexit
  %indvars.iv43 = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next44, %for.cond2.loopexit ]
  %indvars.iv = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next, %for.cond2.loopexit ]
  %indvars.iv.next44 = add nuw nsw i64 %indvars.iv43, 1
  %cmp737 = icmp ult i64 %indvars.iv43, 255
  br i1 %cmp737, label %for.body9.lr.ph, label %for.cond2.loopexit

for.body9.lr.ph:                                  ; preds = %for.body5
  %arrayidx13 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv43
  br label %for.body9

for.body9:                                        ; preds = %for.body9.lr.ph, %for.body9
  %indvars.iv41 = phi i64 [ %indvars.iv, %for.body9.lr.ph ], [ %indvars.iv.next42, %for.body9 ]
  %arrayidx11 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv43, i64 %indvars.iv41
  %0 = load float, float* %arrayidx11, align 4, !tbaa !4
  %1 = load float, float* %arrayidx13, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx15 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv41
  %2 = load float, float* %arrayidx15, align 4, !tbaa !4
  %sub = fsub float %2, %mul
  store float %sub, float* %arrayidx15, align 4, !tbaa !4
  %indvars.iv.next42 = add nuw nsw i64 %indvars.iv41, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next42, 256
  br i1 %exitcond.not, label %for.cond2.loopexit, label %for.body9, !llvm.loop !26
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s1115(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1115, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.050 = phi i32 [ 0, %entry ], [ %inc29, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call31 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call32 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1115, i64 0, i64 0)) #11
  ret float %call32

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv51 = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next52, %for.cond.cleanup8 ]
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call27 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc29 = add nuw nsw i32 %nl.050, 1
  %exitcond54.not = icmp eq i32 %inc29, 39000
  br i1 %exitcond54.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !27

for.cond.cleanup8:                                ; preds = %for.body9
  %indvars.iv.next52 = add nuw nsw i64 %indvars.iv51, 1
  %exitcond53.not = icmp eq i64 %indvars.iv.next52, 256
  br i1 %exitcond53.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !28

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %indvars.iv = phi i64 [ 0, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %arrayidx11 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv51, i64 %indvars.iv
  %0 = load float, float* %arrayidx11, align 4, !tbaa !4
  %arrayidx15 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 %indvars.iv, i64 %indvars.iv51
  %1 = load float, float* %arrayidx15, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx19 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv51, i64 %indvars.iv
  %2 = load float, float* %arrayidx19, align 4, !tbaa !4
  %add = fadd float %mul, %2
  store float %add, float* %arrayidx11, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !29
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s116(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s116, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.077 = phi i32 [ 0, %entry ], [ %inc, %for.cond.cleanup4 ]
  %.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), align 64, !tbaa !4
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call54 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call55 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s116, i64 0, i64 0)) #11
  ret float %call55

for.cond.cleanup4:                                ; preds = %for.body5
  %call51 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc = add nuw nsw i32 %nl.077, 1
  %exitcond.not = icmp eq i32 %inc, 1000000
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !30

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %0 = phi float [ %.pre, %for.cond2.preheader ], [ %9, %for.body5 ]
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %1 = add nuw nsw i64 %indvars.iv, 1
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %1
  %2 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %mul = fmul float %2, %0
  store float %mul, float* %arrayidx7, align 4, !tbaa !4
  %3 = add nuw nsw i64 %indvars.iv, 2
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %3
  %4 = load float, float* %arrayidx12, align 4, !tbaa !4
  %mul16 = fmul float %2, %4
  store float %mul16, float* %arrayidx, align 4, !tbaa !4
  %5 = add nuw nsw i64 %indvars.iv, 3
  %arrayidx22 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %5
  %6 = load float, float* %arrayidx22, align 4, !tbaa !4
  %mul26 = fmul float %4, %6
  store float %mul26, float* %arrayidx12, align 4, !tbaa !4
  %7 = add nuw nsw i64 %indvars.iv, 4
  %arrayidx32 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %7
  %8 = load float, float* %arrayidx32, align 4, !tbaa !4
  %mul36 = fmul float %6, %8
  store float %mul36, float* %arrayidx22, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 5
  %arrayidx42 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next
  %9 = load float, float* %arrayidx42, align 4, !tbaa !4
  %mul46 = fmul float %8, %9
  store float %mul46, float* %arrayidx32, align 4, !tbaa !4
  %cmp3 = icmp ult i64 %indvars.iv, 31990
  br i1 %cmp3, label %for.body5, label %for.cond.cleanup4, !llvm.loop !31
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s118(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s118, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.042 = phi i32 [ 0, %entry ], [ %inc23, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call25 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call26 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s118, i64 0, i64 0)) #11
  ret float %call26

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv44 = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next45, %for.cond.cleanup8 ]
  %arrayidx17 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv44
  %.pre = load float, float* %arrayidx17, align 4, !tbaa !4
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call21 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc23 = add nuw nsw i32 %nl.042, 1
  %exitcond47.not = icmp eq i32 %inc23, 78000
  br i1 %exitcond47.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !32

for.cond.cleanup8:                                ; preds = %for.body9
  %indvars.iv.next45 = add nuw nsw i64 %indvars.iv44, 1
  %exitcond46.not = icmp eq i64 %indvars.iv.next45, 256
  br i1 %exitcond46.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !33

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %0 = phi float [ %.pre, %for.cond6.preheader ], [ %add, %for.body9 ]
  %indvars.iv = phi i64 [ 0, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %arrayidx11 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv, i64 %indvars.iv44
  %1 = load float, float* %arrayidx11, align 4, !tbaa !4
  %2 = xor i64 %indvars.iv, -1
  %3 = add nsw i64 %indvars.iv44, %2
  %arrayidx15 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %3
  %4 = load float, float* %arrayidx15, align 4, !tbaa !4
  %mul = fmul float %1, %4
  %add = fadd float %0, %mul
  store float %add, float* %arrayidx17, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, %indvars.iv44
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !34
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s119(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s119, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.045 = phi i32 [ 0, %entry ], [ %inc26, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call28 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call29 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s119, i64 0, i64 0)) #11
  ret float %call29

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv47 = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next48, %for.cond.cleanup8 ]
  %0 = add nsw i64 %indvars.iv47, -1
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call24 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc26 = add nuw nsw i32 %nl.045, 1
  %exitcond51.not = icmp eq i32 %inc26, 78000
  br i1 %exitcond51.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !35

for.cond.cleanup8:                                ; preds = %for.body9
  %indvars.iv.next48 = add nuw nsw i64 %indvars.iv47, 1
  %exitcond50.not = icmp eq i64 %indvars.iv.next48, 256
  br i1 %exitcond50.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !36

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %indvars.iv = phi i64 [ 1, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %1 = add nsw i64 %indvars.iv, -1
  %arrayidx12 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %0, i64 %1
  %2 = load float, float* %arrayidx12, align 4, !tbaa !4
  %arrayidx16 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv47, i64 %indvars.iv
  %3 = load float, float* %arrayidx16, align 4, !tbaa !4
  %add = fadd float %2, %3
  %arrayidx20 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv47, i64 %indvars.iv
  store float %add, float* %arrayidx20, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !37
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s1119(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1119, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.044 = phi i32 [ 0, %entry ], [ %inc25, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call27 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call28 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1119, i64 0, i64 0)) #11
  ret float %call28

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv45 = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next46, %for.cond.cleanup8 ]
  %0 = add nsw i64 %indvars.iv45, -1
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call23 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc25 = add nuw nsw i32 %nl.044, 1
  %exitcond49.not = icmp eq i32 %inc25, 78000
  br i1 %exitcond49.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !38

for.cond.cleanup8:                                ; preds = %for.body9
  %indvars.iv.next46 = add nuw nsw i64 %indvars.iv45, 1
  %exitcond48.not = icmp eq i64 %indvars.iv.next46, 256
  br i1 %exitcond48.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !39

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %indvars.iv = phi i64 [ 0, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %arrayidx11 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %0, i64 %indvars.iv
  %1 = load float, float* %arrayidx11, align 4, !tbaa !4
  %arrayidx15 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv45, i64 %indvars.iv
  %2 = load float, float* %arrayidx15, align 4, !tbaa !4
  %add = fadd float %1, %2
  %arrayidx19 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv45, i64 %indvars.iv
  store float %add, float* %arrayidx19, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !40
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s121(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s121, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.027 = phi i32 [ 0, %entry ], [ %inc13, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call15 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call16 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s121, i64 0, i64 0)) #11
  ret float %call16

for.cond.cleanup4:                                ; preds = %for.body5
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc13 = add nuw nsw i32 %nl.027, 1
  %exitcond28.not = icmp eq i32 %inc13, 300000
  br i1 %exitcond28.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !41

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add8 = fadd float %0, %1
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add8, float* %arrayidx10, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !42
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s122(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to %struct.anon**
  %1 = load %struct.anon*, %struct.anon** %0, align 8, !tbaa !43
  %a = getelementptr inbounds %struct.anon, %struct.anon* %1, i64 0, i32 0
  %2 = load i32, i32* %a, align 4, !tbaa !48
  %b = getelementptr inbounds %struct.anon, %struct.anon* %1, i64 0, i32 1
  %3 = load i32, i32* %b, align 4, !tbaa !51
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s122, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  %cmp330 = icmp slt i32 %2, 32001
  %4 = add i32 %2, -1
  %5 = sext i32 %4 to i64
  %6 = sext i32 %3 to i64
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call15 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s122, i64 0, i64 0)) #11
  ret float %call15

for.body:                                         ; preds = %entry, %for.cond.cleanup4
  %nl.033 = phi i32 [ 0, %entry ], [ %inc, %for.cond.cleanup4 ]
  br i1 %cmp330, label %for.body5, label %for.cond.cleanup4

for.cond.cleanup4:                                ; preds = %for.body5, %for.body
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc = add nuw nsw i32 %nl.033, 1
  %exitcond.not = icmp eq i32 %inc, 100000
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.body, !llvm.loop !52

for.body5:                                        ; preds = %for.body, %for.body5
  %indvars.iv34 = phi i64 [ %indvars.iv.next35, %for.body5 ], [ 0, %for.body ]
  %indvars.iv = phi i64 [ %indvars.iv.next, %for.body5 ], [ %5, %for.body ]
  %indvars.iv.next35 = add nuw nsw i64 %indvars.iv34, 1
  %7 = sub nsw i64 31999, %indvars.iv34
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %7
  %8 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx8 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %9 = load float, float* %arrayidx8, align 4, !tbaa !4
  %add9 = fadd float %8, %9
  store float %add9, float* %arrayidx8, align 4, !tbaa !4
  %indvars.iv.next = add i64 %indvars.iv, %6
  %cmp3 = icmp slt i64 %indvars.iv.next, 32000
  br i1 %cmp3, label %for.body5, label %for.cond.cleanup4, !llvm.loop !53
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s123(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s123, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.051 = phi i32 [ 0, %entry ], [ %inc29, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call31 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call32 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s123, i64 0, i64 0)) #11
  ret float %call32

for.cond.cleanup4:                                ; preds = %for.inc
  %call27 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc29 = add nuw nsw i32 %nl.051, 1
  %exitcond52.not = icmp eq i32 %inc29, 100000
  br i1 %exitcond52.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !54

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %j.049 = phi i32 [ -1, %for.cond2.preheader ], [ %j.1, %for.inc ]
  %inc = add nsw i32 %j.049, 1
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul = fmul float %1, %2
  %add = fadd float %0, %mul
  %idxprom10 = sext i32 %inc to i64
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %idxprom10
  store float %add, float* %arrayidx11, align 4, !tbaa !4
  %arrayidx13 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx13, align 4, !tbaa !4
  %cmp14 = fcmp ogt float %3, 0.000000e+00
  br i1 %cmp14, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body5
  %inc15 = add nsw i32 %j.049, 2
  %add23 = fadd float %mul, %3
  %idxprom24 = sext i32 %inc15 to i64
  %arrayidx25 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %idxprom24
  store float %add23, float* %arrayidx25, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %for.body5, %if.then
  %j.1 = phi i32 [ %inc15, %if.then ], [ %inc, %for.body5 ]
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 16000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !55
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s124(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s124, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.051 = phi i32 [ 0, %entry ], [ %inc29, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call31 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call32 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s124, i64 0, i64 0)) #11
  ret float %call32

for.cond.cleanup4:                                ; preds = %for.inc
  %call27 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc29 = add nuw nsw i32 %nl.051, 1
  %exitcond52.not = icmp eq i32 %inc29, 100000
  br i1 %exitcond52.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !56

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %j.049 = phi i32 [ -1, %for.cond2.preheader ], [ %inc, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp ogt float %0, 0.000000e+00
  %inc = add nsw i32 %j.049, 1
  br i1 %cmp6, label %for.inc, label %if.else

if.else:                                          ; preds = %for.body5
  %arrayidx17 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx17, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %for.body5, %if.else
  %.sink = phi float [ %1, %if.else ], [ %0, %for.body5 ]
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx10, align 4, !tbaa !4
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx12, align 4, !tbaa !4
  %mul = fmul float %2, %3
  %add = fadd float %.sink, %mul
  %idxprom13 = sext i32 %inc to i64
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %idxprom13
  store float %add, float* %arrayidx14, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !57
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s125(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s125, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.051 = phi i32 [ 0, %entry ], [ %inc28, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call30 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call31 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s125, i64 0, i64 0)) #11
  ret float %call31

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv54 = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next55, %for.cond.cleanup8 ]
  %k.049 = phi i64 [ -1, %for.cond2.preheader ], [ %indvars.iv.next53, %for.cond.cleanup8 ]
  %sext = shl i64 %k.049, 32
  %0 = ashr exact i64 %sext, 32
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call26 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc28 = add nuw nsw i32 %nl.051, 1
  %exitcond57.not = icmp eq i32 %inc28, 39000
  br i1 %exitcond57.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !58

for.cond.cleanup8:                                ; preds = %for.body9
  %indvars.iv.next55 = add nuw nsw i64 %indvars.iv54, 1
  %exitcond56.not = icmp eq i64 %indvars.iv.next55, 256
  br i1 %exitcond56.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !59

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %indvars.iv52 = phi i64 [ %0, %for.cond6.preheader ], [ %indvars.iv.next53, %for.body9 ]
  %indvars.iv = phi i64 [ 0, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %indvars.iv.next53 = add nsw i64 %indvars.iv52, 1
  %arrayidx11 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv54, i64 %indvars.iv
  %1 = load float, float* %arrayidx11, align 4, !tbaa !4
  %arrayidx15 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv54, i64 %indvars.iv
  %2 = load float, float* %arrayidx15, align 4, !tbaa !4
  %arrayidx19 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 %indvars.iv54, i64 %indvars.iv
  %3 = load float, float* %arrayidx19, align 4, !tbaa !4
  %mul = fmul float %2, %3
  %add = fadd float %1, %mul
  %arrayidx21 = getelementptr inbounds [65536 x float], [65536 x float]* @flat_2d_array, i64 0, i64 %indvars.iv.next53
  store float %add, float* %arrayidx21, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !60
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s126(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s126, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.054 = phi i32 [ 0, %entry ], [ %inc30, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call32 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call33 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s126, i64 0, i64 0)) #11
  ret float %call33

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv59 = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next60, %for.cond.cleanup8 ]
  %k.052 = phi i32 [ 1, %for.cond2.preheader ], [ %inc24, %for.cond.cleanup8 ]
  %0 = sext i32 %k.052 to i64
  %1 = add i32 %k.052, 255
  %arrayidx11.phi.trans.insert = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0, i64 %indvars.iv59
  %.pre = load float, float* %arrayidx11.phi.trans.insert, align 4, !tbaa !4
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call28 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc30 = add nuw nsw i32 %nl.054, 1
  %exitcond62.not = icmp eq i32 %inc30, 3900
  br i1 %exitcond62.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !61

for.cond.cleanup8:                                ; preds = %for.body9
  %2 = trunc i64 %indvars.iv56 to i32
  %inc24 = add nsw i32 %2, 2
  %indvars.iv.next60 = add nuw nsw i64 %indvars.iv59, 1
  %exitcond61.not = icmp eq i64 %indvars.iv.next60, 256
  br i1 %exitcond61.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !62

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %3 = phi float [ %.pre, %for.cond6.preheader ], [ %add, %for.body9 ]
  %indvars.iv56 = phi i64 [ %0, %for.cond6.preheader ], [ %indvars.iv.next57, %for.body9 ]
  %indvars.iv = phi i64 [ 1, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %4 = add nsw i64 %indvars.iv56, -1
  %arrayidx14 = getelementptr inbounds [65536 x float], [65536 x float]* @flat_2d_array, i64 0, i64 %4
  %5 = load float, float* %arrayidx14, align 4, !tbaa !4
  %arrayidx18 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 %indvars.iv, i64 %indvars.iv59
  %6 = load float, float* %arrayidx18, align 4, !tbaa !4
  %mul = fmul float %5, %6
  %add = fadd float %3, %mul
  %arrayidx22 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv, i64 %indvars.iv59
  store float %add, float* %arrayidx22, align 4, !tbaa !4
  %indvars.iv.next57 = add nsw i64 %indvars.iv56, 1
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %lftr.wideiv = trunc i64 %indvars.iv.next57 to i32
  %exitcond.not = icmp eq i32 %1, %lftr.wideiv
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !63
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s127(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s127, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.047 = phi i32 [ 0, %entry ], [ %inc26, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call28 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call29 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s127, i64 0, i64 0)) #11
  ret float %call29

for.cond.cleanup4:                                ; preds = %for.body5
  %call24 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc26 = add nuw nsw i32 %nl.047, 1
  %exitcond51.not = icmp eq i32 %inc26, 200000
  br i1 %exitcond51.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !64

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv48 = phi i64 [ -1, %for.cond2.preheader ], [ %indvars.iv.next49, %for.body5 ]
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %0 = add nsw i64 %indvars.iv48, 1
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul = fmul float %2, %3
  %add = fadd float %1, %mul
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %0
  store float %add, float* %arrayidx11, align 4, !tbaa !4
  %indvars.iv.next49 = add nsw i64 %indvars.iv48, 2
  %arrayidx18 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx18, align 4, !tbaa !4
  %mul19 = fmul float %3, %4
  %add20 = fadd float %1, %mul19
  %arrayidx22 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next49
  store float %add20, float* %arrayidx22, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 16000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !65
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s128(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s128, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.039 = phi i32 [ 0, %entry ], [ %inc20, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call22 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call23 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s128, i64 0, i64 0)) #11
  ret float %call23

for.cond.cleanup4:                                ; preds = %for.body5
  %call18 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 1.000000e+00) #11
  %inc20 = add nuw nsw i32 %nl.039, 1
  %exitcond43.not = icmp eq i32 %inc20, 200000
  br i1 %exitcond43.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !66

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv40 = phi i64 [ -1, %for.cond2.preheader ], [ %indvars.iv.next41, %for.body5 ]
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %0 = add nsw i64 %indvars.iv40, 1
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %0
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx7, align 4, !tbaa !4
  %sub = fsub float %1, %2
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %sub, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next41 = add nsw i64 %indvars.iv40, 2
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %0
  %3 = load float, float* %arrayidx14, align 4, !tbaa !4
  %add15 = fadd float %sub, %3
  store float %add15, float* %arrayidx, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 16000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !67
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s131(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s131, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.027 = phi i32 [ 0, %entry ], [ %inc13, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call15 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call16 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s131, i64 0, i64 0)) #11
  ret float %call16

for.cond.cleanup4:                                ; preds = %for.body5
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc13 = add nuw nsw i32 %nl.027, 1
  %exitcond28.not = icmp eq i32 %inc13, 500000
  br i1 %exitcond28.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !68

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add8 = fadd float %0, %1
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add8, float* %arrayidx10, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !69
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s132(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s132, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.034 = phi i32 [ 0, %entry ], [ %inc17, %for.cond.cleanup4 ]
  %0 = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 1), align 4, !tbaa !4
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call19 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call20 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s132, i64 0, i64 0)) #11
  ret float %call20

for.cond.cleanup4:                                ; preds = %for.body5
  %call15 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc17 = add nuw nsw i32 %nl.034, 1
  %exitcond36.not = icmp eq i32 %inc17, 40000000
  br i1 %exitcond36.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !70

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %1 = add nsw i64 %indvars.iv, -1
  %arrayidx7 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 1, i64 %1
  %2 = load float, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul = fmul float %3, %0
  %add10 = fadd float %2, %mul
  %arrayidx14 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0, i64 %indvars.iv
  store float %add10, float* %arrayidx14, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !71
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s141(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s141, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.050 = phi i32 [ 0, %entry ], [ %inc26, %for.cond.cleanup4 ]
  br label %for.body13.lr.ph

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call28 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call29 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s141, i64 0, i64 0)) #11
  ret float %call29

for.cond2.loopexit:                               ; preds = %for.body13
  %exitcond54.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond54.not, label %for.cond.cleanup4, label %for.body13.lr.ph, !llvm.loop !72

for.cond.cleanup4:                                ; preds = %for.cond2.loopexit
  %call24 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc26 = add nuw nsw i32 %nl.050, 1
  %exitcond55.not = icmp eq i32 %inc26, 78000
  br i1 %exitcond55.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !73

for.body13.lr.ph:                                 ; preds = %for.cond2.loopexit, %for.cond2.preheader
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.cond2.loopexit ]
  %i.049 = phi i32 [ 0, %for.cond2.preheader ], [ %add, %for.cond2.loopexit ]
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %add = add nuw nsw i32 %i.049, 1
  %0 = trunc i64 %indvars.iv to i32
  %1 = mul i32 %add, %0
  %div = lshr i32 %1, 1
  %2 = trunc i64 %indvars.iv to i32
  %sub9 = add nuw nsw i32 %div, %2
  br label %for.body13

for.body13:                                       ; preds = %for.body13.lr.ph, %for.body13
  %indvars.iv51 = phi i64 [ %indvars.iv, %for.body13.lr.ph ], [ %indvars.iv.next52, %for.body13 ]
  %k.047 = phi i32 [ %sub9, %for.body13.lr.ph ], [ %add20, %for.body13 ]
  %arrayidx15 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv51, i64 %indvars.iv
  %3 = load float, float* %arrayidx15, align 4, !tbaa !4
  %idxprom16 = zext i32 %k.047 to i64
  %arrayidx17 = getelementptr inbounds [65536 x float], [65536 x float]* @flat_2d_array, i64 0, i64 %idxprom16
  %4 = load float, float* %arrayidx17, align 4, !tbaa !4
  %add18 = fadd float %3, %4
  store float %add18, float* %arrayidx17, align 4, !tbaa !4
  %indvars.iv.next52 = add nuw nsw i64 %indvars.iv51, 1
  %5 = trunc i64 %indvars.iv.next52 to i32
  %add20 = add nuw nsw i32 %k.047, %5
  %exitcond.not = icmp eq i64 %indvars.iv.next52, 256
  br i1 %exitcond.not, label %for.cond2.loopexit, label %for.body13, !llvm.loop !74
}

; Function Attrs: nofree norecurse nounwind optsize uwtable
define dso_local void @s151s(float* nocapture %a, float* nocapture readonly %b, i32 %m) local_unnamed_addr #4 {
entry:
  %0 = sext i32 %m to i64
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.body
  ret void

for.body:                                         ; preds = %entry, %for.body
  %indvars.iv = phi i64 [ 0, %entry ], [ %indvars.iv.next, %for.body ]
  %1 = add nsw i64 %indvars.iv, %0
  %arrayidx = getelementptr inbounds float, float* %a, i64 %1
  %2 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx2 = getelementptr inbounds float, float* %b, i64 %indvars.iv
  %3 = load float, float* %arrayidx2, align 4, !tbaa !4
  %add3 = fadd float %2, %3
  %arrayidx5 = getelementptr inbounds float, float* %a, i64 %indvars.iv
  store float %add3, float* %arrayidx5, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.body, !llvm.loop !75
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s151(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s151, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.body.i.preheader

for.body.i.preheader:                             ; preds = %entry, %s151s.exit
  %nl.08 = phi i32 [ 0, %entry ], [ %inc, %s151s.exit ]
  br label %for.body.i

for.cond.cleanup:                                 ; preds = %s151s.exit
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call3 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call4 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s151, i64 0, i64 0)) #11
  ret float %call4

for.body.i:                                       ; preds = %for.body.i.preheader, %for.body.i
  %indvars.iv.i = phi i64 [ %0, %for.body.i ], [ 0, %for.body.i.preheader ]
  %0 = add nuw nsw i64 %indvars.iv.i, 1
  %arrayidx.i = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %0
  %1 = load float, float* %arrayidx.i, align 4, !tbaa !4
  %arrayidx2.i = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv.i
  %2 = load float, float* %arrayidx2.i, align 4, !tbaa !4
  %add3.i = fadd float %1, %2
  %arrayidx5.i = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.i
  store float %add3.i, float* %arrayidx5.i, align 4, !tbaa !4
  %exitcond.not.i = icmp eq i64 %0, 31999
  br i1 %exitcond.not.i, label %s151s.exit, label %for.body.i, !llvm.loop !75

s151s.exit:                                       ; preds = %for.body.i
  %call2 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc = add nuw nsw i32 %nl.08, 1
  %exitcond.not = icmp eq i32 %inc, 500000
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.body.i.preheader, !llvm.loop !76
}

; Function Attrs: nofree norecurse nounwind optsize uwtable
define dso_local void @s152s(float* nocapture %a, float* nocapture readonly %b, float* nocapture readonly %c, i32 %i) local_unnamed_addr #4 {
entry:
  %idxprom = sext i32 %i to i64
  %arrayidx = getelementptr inbounds float, float* %b, i64 %idxprom
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx2 = getelementptr inbounds float, float* %c, i64 %idxprom
  %1 = load float, float* %arrayidx2, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx4 = getelementptr inbounds float, float* %a, i64 %idxprom
  %2 = load float, float* %arrayidx4, align 4, !tbaa !4
  %add = fadd float %2, %mul
  store float %add, float* %arrayidx4, align 4, !tbaa !4
  ret void
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s152(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s152, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.027 = phi i32 [ 0, %entry ], [ %inc12, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call15 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s152, i64 0, i64 0)) #11
  ret float %call15

for.cond.cleanup4:                                ; preds = %for.body5
  %call10 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc12 = add nuw nsw i32 %nl.027, 1
  %exitcond28.not = icmp eq i32 %inc12, 100000
  br i1 %exitcond28.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !77

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  store float %mul, float* %arrayidx9, align 4, !tbaa !4
  %arrayidx2.i = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx2.i, align 4, !tbaa !4
  %mul.i = fmul float %mul, %2
  %arrayidx4.i = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx4.i, align 4, !tbaa !4
  %add.i = fadd float %3, %mul.i
  store float %add.i, float* %arrayidx4.i, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !78
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s161(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s161, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.048 = phi i32 [ 0, %entry ], [ %inc28, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call30 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call31 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s161, i64 0, i64 0)) #11
  ret float %call31

for.cond.cleanup4:                                ; preds = %for.inc
  %call26 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc28 = add nuw nsw i32 %nl.048, 1
  %exitcond50.not = icmp eq i32 %inc28, 50000
  br i1 %exitcond50.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !79

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next.pre-phi, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp olt float %0, 0.000000e+00
  br i1 %cmp6, label %L20, label %if.end

if.end:                                           ; preds = %for.body5
  %arrayidx8 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx8, align 4, !tbaa !4
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx10, align 4, !tbaa !4
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx12, align 4, !tbaa !4
  %mul = fmul float %2, %3
  %add = fadd float %1, %mul
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx14, align 4, !tbaa !4
  %.pre = add nuw nsw i64 %indvars.iv, 1
  br label %for.inc

L20:                                              ; preds = %for.body5
  %arrayidx16 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx16, align 4, !tbaa !4
  %arrayidx18 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %5 = load float, float* %arrayidx18, align 4, !tbaa !4
  %mul21 = fmul float %5, %5
  %add22 = fadd float %4, %mul21
  %6 = add nuw nsw i64 %indvars.iv, 1
  %arrayidx25 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %6
  store float %add22, float* %arrayidx25, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %if.end, %L20
  %indvars.iv.next.pre-phi = phi i64 [ %.pre, %if.end ], [ %6, %L20 ]
  %exitcond.not = icmp eq i64 %indvars.iv.next.pre-phi, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !80
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s1161(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1161, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.046 = phi i32 [ 0, %entry ], [ %inc27, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call29 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call30 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1161, i64 0, i64 0)) #11
  ret float %call30

for.cond.cleanup4:                                ; preds = %for.inc
  %call25 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc27 = add nuw nsw i32 %nl.046, 1
  %exitcond47.not = icmp eq i32 %inc27, 100000
  br i1 %exitcond47.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !81

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp olt float %0, 0.000000e+00
  br i1 %cmp6, label %L20, label %if.end

if.end:                                           ; preds = %for.body5
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx10, align 4, !tbaa !4
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx12, align 4, !tbaa !4
  %mul = fmul float %1, %2
  br label %for.inc

L20:                                              ; preds = %for.body5
  %arrayidx16 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx16, align 4, !tbaa !4
  %arrayidx18 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx18, align 4, !tbaa !4
  %mul21 = fmul float %4, %4
  br label %for.inc

for.inc:                                          ; preds = %if.end, %L20
  %mul.sink = phi float [ %mul, %if.end ], [ %mul21, %L20 ]
  %.sink = phi float [ %0, %if.end ], [ %3, %L20 ]
  %a.sink = phi [32000 x float]* [ @a, %if.end ], [ @b, %L20 ]
  %add = fadd float %.sink, %mul.sink
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* %a.sink, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx14, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !82
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s162(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to i32**
  %1 = load i32*, i32** %0, align 8, !tbaa !43
  %2 = load i32, i32* %1, align 4, !tbaa !83
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s162, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  %cmp2 = icmp sgt i32 %2, 0
  %3 = sext i32 %2 to i64
  br label %for.body

for.cond.cleanup:                                 ; preds = %if.end
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call18 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call19 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s162, i64 0, i64 0)) #11
  ret float %call19

for.body:                                         ; preds = %entry, %if.end
  %nl.033 = phi i32 [ 0, %entry ], [ %inc16, %if.end ]
  br i1 %cmp2, label %for.body6, label %if.end

for.body6:                                        ; preds = %for.body, %for.body6
  %indvars.iv = phi i64 [ %indvars.iv.next, %for.body6 ], [ 0, %for.body ]
  %4 = add nsw i64 %indvars.iv, %3
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %4
  %5 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx8 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %6 = load float, float* %arrayidx8, align 4, !tbaa !4
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %7 = load float, float* %arrayidx10, align 4, !tbaa !4
  %mul = fmul float %6, %7
  %add11 = fadd float %5, %mul
  %arrayidx13 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add11, float* %arrayidx13, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %if.end, label %for.body6, !llvm.loop !84

if.end:                                           ; preds = %for.body6, %for.body
  %call14 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc16 = add nuw nsw i32 %nl.033, 1
  %exitcond35.not = icmp eq i32 %inc16, 100000
  br i1 %exitcond35.not, label %for.cond.cleanup, label %for.body, !llvm.loop !85
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s171(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to i32**
  %1 = load i32*, i32** %0, align 8, !tbaa !43
  %2 = load i32, i32* %1, align 4, !tbaa !83
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s171, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  %3 = sext i32 %2 to i64
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.025 = phi i32 [ 0, %entry ], [ %inc11, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call13 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call14 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s171, i64 0, i64 0)) #11
  ret float %call14

for.cond.cleanup4:                                ; preds = %for.body5
  %call9 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc11 = add nuw nsw i32 %nl.025, 1
  %exitcond27.not = icmp eq i32 %inc11, 100000
  br i1 %exitcond27.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !86

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx, align 4, !tbaa !4
  %5 = mul nsw i64 %indvars.iv, %3
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %5
  %6 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %4, %6
  store float %add, float* %arrayidx7, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !87
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s172(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to %struct.anon.0**
  %1 = load %struct.anon.0*, %struct.anon.0** %0, align 8, !tbaa !43
  %a = getelementptr inbounds %struct.anon.0, %struct.anon.0* %1, i64 0, i32 0
  %2 = load i32, i32* %a, align 4, !tbaa !48
  %b = getelementptr inbounds %struct.anon.0, %struct.anon.0* %1, i64 0, i32 1
  %3 = load i32, i32* %b, align 4, !tbaa !51
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s172, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  %cmp326 = icmp slt i32 %2, 32001
  %4 = add i32 %2, -1
  %5 = sext i32 %4 to i64
  %6 = sext i32 %3 to i64
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call12 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call13 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s172, i64 0, i64 0)) #11
  ret float %call13

for.body:                                         ; preds = %entry, %for.cond.cleanup4
  %nl.028 = phi i32 [ 0, %entry ], [ %inc, %for.cond.cleanup4 ]
  br i1 %cmp326, label %for.body5, label %for.cond.cleanup4

for.cond.cleanup4:                                ; preds = %for.body5, %for.body
  %call9 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc = add nuw nsw i32 %nl.028, 1
  %exitcond.not = icmp eq i32 %inc, 100000
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.body, !llvm.loop !88

for.body5:                                        ; preds = %for.body, %for.body5
  %indvars.iv = phi i64 [ %indvars.iv.next, %for.body5 ], [ %5, %for.body ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %7 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %8 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %7, %8
  store float %add, float* %arrayidx7, align 4, !tbaa !4
  %indvars.iv.next = add i64 %indvars.iv, %6
  %cmp3 = icmp slt i64 %indvars.iv.next, 32000
  br i1 %cmp3, label %for.body5, label %for.cond.cleanup4, !llvm.loop !89
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s173(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s173, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.027 = phi i32 [ 0, %entry ], [ %inc13, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call15 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call16 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s173, i64 0, i64 0)) #11
  ret float %call16

for.cond.cleanup4:                                ; preds = %for.body5
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc13 = add nuw nsw i32 %nl.027, 1
  %exitcond29.not = icmp eq i32 %inc13, 1000000
  br i1 %exitcond29.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !90

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %0, %1
  %2 = add nuw nsw i64 %indvars.iv, 16000
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %2
  store float %add, float* %arrayidx10, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 16000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !91
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s174(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to i32**
  %1 = load i32*, i32** %0, align 8, !tbaa !43
  %2 = load i32, i32* %1, align 4, !tbaa !83
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s174, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  %cmp328 = icmp sgt i32 %2, 0
  %3 = sext i32 %2 to i64
  %wide.trip.count = zext i32 %2 to i64
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.030 = phi i32 [ 0, %entry ], [ %inc13, %for.cond.cleanup4 ]
  br i1 %cmp328, label %for.body5, label %for.cond.cleanup4

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call15 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call16 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s174, i64 0, i64 0)) #11
  ret float %call16

for.cond.cleanup4:                                ; preds = %for.body5, %for.cond2.preheader
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc13 = add nuw nsw i32 %nl.030, 1
  %exitcond32.not = icmp eq i32 %inc13, 1000000
  br i1 %exitcond32.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !92

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ %indvars.iv.next, %for.body5 ], [ 0, %for.cond2.preheader ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %5 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %4, %5
  %6 = add nsw i64 %indvars.iv, %3
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %6
  store float %add, float* %arrayidx10, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, %wide.trip.count
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !93
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s175(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to i32**
  %1 = load i32*, i32** %0, align 8, !tbaa !43
  %2 = load i32, i32* %1, align 4, !tbaa !83
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s175, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  %3 = sext i32 %2 to i64
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.030 = phi i32 [ 0, %entry ], [ %inc14, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call16 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call17 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s175, i64 0, i64 0)) #11
  ret float %call17

for.cond.cleanup4:                                ; preds = %for.body5
  %call12 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc14 = add nuw nsw i32 %nl.030, 1
  %exitcond.not = icmp eq i32 %inc14, 100000
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !94

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %indvars.iv.next = add nsw i64 %indvars.iv, %3
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next
  %4 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %5 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add8 = fadd float %4, %5
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add8, float* %arrayidx10, align 4, !tbaa !4
  %cmp3 = icmp slt i64 %indvars.iv.next, 31999
  br i1 %cmp3, label %for.body5, label %for.cond.cleanup4, !llvm.loop !95
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s176(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s176, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.040 = phi i32 [ 0, %entry ], [ %inc21, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call23 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call24 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s176, i64 0, i64 0)) #11
  ret float %call24

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv42 = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next43, %for.cond.cleanup8 ]
  %0 = sub nuw nsw i64 15999, %indvars.iv42
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv42
  %1 = load float, float* %arrayidx12, align 4, !tbaa !4
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call19 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc21 = add nuw nsw i32 %nl.040, 1
  %exitcond46.not = icmp eq i32 %inc21, 12
  br i1 %exitcond46.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !96

for.cond.cleanup8:                                ; preds = %for.body9
  %indvars.iv.next43 = add nuw nsw i64 %indvars.iv42, 1
  %exitcond45.not = icmp eq i64 %indvars.iv.next43, 16000
  br i1 %exitcond45.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !97

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %indvars.iv = phi i64 [ 0, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %2 = add nuw nsw i64 %0, %indvars.iv
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %2
  %3 = load float, float* %arrayidx, align 4, !tbaa !4
  %mul = fmul float %3, %1
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx14, align 4, !tbaa !4
  %add15 = fadd float %4, %mul
  store float %add15, float* %arrayidx14, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 16000
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !98
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s211(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s211, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.043 = phi i32 [ 0, %entry ], [ %inc25, %for.cond.cleanup4 ]
  %.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), align 64, !tbaa !4
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call27 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call28 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s211, i64 0, i64 0)) #11
  ret float %call28

for.cond.cleanup4:                                ; preds = %for.body5
  %call23 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc25 = add nuw nsw i32 %nl.043, 1
  %exitcond45.not = icmp eq i32 %inc25, 100000
  br i1 %exitcond45.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !99

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %0 = phi float [ %.pre, %for.cond2.preheader ], [ %sub20, %for.body5 ]
  %indvars.iv = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul = fmul float %1, %2
  %add = fadd float %0, %mul
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx11, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv.next
  %3 = load float, float* %arrayidx14, align 4, !tbaa !4
  %arrayidx16 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx16, align 4, !tbaa !4
  %mul19 = fmul float %2, %4
  %sub20 = fsub float %3, %mul19
  %arrayidx22 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  store float %sub20, float* %arrayidx22, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !100
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s212(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s212, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.033 = phi i32 [ 0, %entry ], [ %inc18, %for.cond.cleanup4 ]
  %.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), align 64, !tbaa !4
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call20 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call21 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s212, i64 0, i64 0)) #11
  ret float %call21

for.cond.cleanup4:                                ; preds = %for.body5
  %call16 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc18 = add nuw nsw i32 %nl.033, 1
  %exitcond34.not = icmp eq i32 %inc18, 100000
  br i1 %exitcond34.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !101

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %0 = phi float [ %.pre, %for.cond2.preheader ], [ %2, %for.body5 ]
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %mul = fmul float %1, %0
  store float %mul, float* %arrayidx7, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx11, align 4, !tbaa !4
  %mul12 = fmul float %2, %3
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx14, align 4, !tbaa !4
  %add15 = fadd float %4, %mul12
  store float %add15, float* %arrayidx14, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !102
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s1213(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1213, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.035 = phi i32 [ 0, %entry ], [ %inc19, %for.cond.cleanup4 ]
  %.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), align 64, !tbaa !4
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call21 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call22 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1213, i64 0, i64 0)) #11
  ret float %call22

for.cond.cleanup4:                                ; preds = %for.body5
  %call17 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc19 = add nuw nsw i32 %nl.035, 1
  %exitcond37.not = icmp eq i32 %inc19, 100000
  br i1 %exitcond37.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !103

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %0 = phi float [ %.pre, %for.cond2.preheader ], [ %mul, %for.body5 ]
  %indvars.iv = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %0, %1
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next
  %2 = load float, float* %arrayidx12, align 4, !tbaa !4
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx14, align 4, !tbaa !4
  %mul = fmul float %2, %3
  %arrayidx16 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  store float %mul, float* %arrayidx16, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !104
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s221(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s221, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.039 = phi i32 [ 0, %entry ], [ %inc22, %for.cond.cleanup4 ]
  %.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), align 64, !tbaa !4
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call24 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call25 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s221, i64 0, i64 0)) #11
  ret float %call25

for.cond.cleanup4:                                ; preds = %for.body5
  %call20 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc22 = add nuw nsw i32 %nl.039, 1
  %exitcond41.not = icmp eq i32 %inc22, 50000
  br i1 %exitcond41.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !105

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %0 = phi float [ %.pre, %for.cond2.preheader ], [ %add17, %for.body5 ]
  %indvars.iv = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %1, %2
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx9, align 4, !tbaa !4
  %add = fadd float %3, %mul
  store float %add, float* %arrayidx9, align 4, !tbaa !4
  %add14 = fadd float %add, %0
  %add17 = fadd float %2, %add14
  %arrayidx19 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  store float %add17, float* %arrayidx19, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !106
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s1221(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1221, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.025 = phi i32 [ 0, %entry ], [ %inc12, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call15 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1221, i64 0, i64 0)) #11
  ret float %call15

for.cond.cleanup4:                                ; preds = %for.body5
  %call10 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc12 = add nuw nsw i32 %nl.025, 1
  %exitcond27.not = icmp eq i32 %inc12, 100000
  br i1 %exitcond27.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !107

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 4, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %0 = add nsw i64 %indvars.iv, -4
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %0
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %1, %2
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !108
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s222(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s222, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.047 = phi i32 [ 0, %entry ], [ %inc28, %for.cond.cleanup4 ]
  %.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), align 64, !tbaa !4
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call30 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call31 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s222, i64 0, i64 0)) #11
  ret float %call31

for.cond.cleanup4:                                ; preds = %for.body5
  %call26 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc28 = add nuw nsw i32 %nl.047, 1
  %exitcond49.not = icmp eq i32 %inc28, 50000
  br i1 %exitcond49.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !109

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %0 = phi float [ %.pre, %for.cond2.preheader ], [ %mul15, %for.body5 ]
  %indvars.iv = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %1, %2
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx9, align 4, !tbaa !4
  %add = fadd float %3, %mul
  %mul15 = fmul float %0, %0
  %arrayidx17 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  store float %mul15, float* %arrayidx17, align 4, !tbaa !4
  %sub25 = fsub float %add, %mul
  store float %sub25, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !110
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s231(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s231, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.044 = phi i32 [ 0, %entry ], [ %inc25, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call27 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call28 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s231, i64 0, i64 0)) #11
  ret float %call28

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv46 = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next47, %for.cond.cleanup8 ]
  %arrayidx11.phi.trans.insert = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0, i64 %indvars.iv46
  %.pre = load float, float* %arrayidx11.phi.trans.insert, align 4, !tbaa !4
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call23 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc25 = add nuw nsw i32 %nl.044, 1
  %exitcond49.not = icmp eq i32 %inc25, 39000
  br i1 %exitcond49.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !111

for.cond.cleanup8:                                ; preds = %for.body9
  %indvars.iv.next47 = add nuw nsw i64 %indvars.iv46, 1
  %exitcond48.not = icmp eq i64 %indvars.iv.next47, 256
  br i1 %exitcond48.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !112

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %0 = phi float [ %.pre, %for.cond6.preheader ], [ %add, %for.body9 ]
  %indvars.iv = phi i64 [ 1, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %arrayidx15 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv, i64 %indvars.iv46
  %1 = load float, float* %arrayidx15, align 4, !tbaa !4
  %add = fadd float %0, %1
  %arrayidx19 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv, i64 %indvars.iv46
  store float %add, float* %arrayidx19, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !113
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s232(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s232, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.052 = phi i32 [ 0, %entry ], [ %inc30, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call32 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call33 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s232, i64 0, i64 0)) #11
  ret float %call33

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv58 = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next59, %for.cond.cleanup8 ]
  %indvars.iv56 = phi i64 [ 2, %for.cond2.preheader ], [ %indvars.iv.next57, %for.cond.cleanup8 ]
  %arrayidx11.phi.trans.insert = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv58, i64 0
  %.pre = load float, float* %arrayidx11.phi.trans.insert, align 64, !tbaa !4
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call28 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 1.000000e+00) #11
  %inc30 = add nuw nsw i32 %nl.052, 1
  %exitcond61.not = icmp eq i32 %inc30, 39000
  br i1 %exitcond61.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !114

for.cond.cleanup8:                                ; preds = %for.body9
  %indvars.iv.next59 = add nuw nsw i64 %indvars.iv58, 1
  %indvars.iv.next57 = add nuw nsw i64 %indvars.iv56, 1
  %exitcond60.not = icmp eq i64 %indvars.iv.next59, 256
  br i1 %exitcond60.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !115

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %0 = phi float [ %.pre, %for.cond6.preheader ], [ %add, %for.body9 ]
  %indvars.iv = phi i64 [ 1, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %mul = fmul float %0, %0
  %arrayidx20 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv58, i64 %indvars.iv
  %1 = load float, float* %arrayidx20, align 4, !tbaa !4
  %add = fadd float %mul, %1
  %arrayidx24 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv58, i64 %indvars.iv
  store float %add, float* %arrayidx24, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, %indvars.iv56
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !116
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s1232(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1232, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.047 = phi i32 [ 0, %entry ], [ %inc25, %for.cond.cleanup4 ]
  br label %for.body9.lr.ph

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call27 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call28 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1232, i64 0, i64 0)) #11
  ret float %call28

for.body9.lr.ph:                                  ; preds = %for.cond.cleanup8, %for.cond2.preheader
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.cond.cleanup8 ]
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call23 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 1.000000e+00) #11
  %inc25 = add nuw nsw i32 %nl.047, 1
  %exitcond51.not = icmp eq i32 %inc25, 39000
  br i1 %exitcond51.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !117

for.cond.cleanup8:                                ; preds = %for.body9
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond50.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond50.not, label %for.cond.cleanup4, label %for.body9.lr.ph, !llvm.loop !118

for.body9:                                        ; preds = %for.body9.lr.ph, %for.body9
  %indvars.iv48 = phi i64 [ %indvars.iv, %for.body9.lr.ph ], [ %indvars.iv.next49, %for.body9 ]
  %arrayidx11 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv48, i64 %indvars.iv
  %0 = load float, float* %arrayidx11, align 4, !tbaa !4
  %arrayidx15 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 %indvars.iv48, i64 %indvars.iv
  %1 = load float, float* %arrayidx15, align 4, !tbaa !4
  %add = fadd float %0, %1
  %arrayidx19 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv48, i64 %indvars.iv
  store float %add, float* %arrayidx19, align 4, !tbaa !4
  %indvars.iv.next49 = add nuw nsw i64 %indvars.iv48, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next49, 256
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !119
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s233(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s233, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.075 = phi i32 [ 0, %entry ], [ %inc47, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call49 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call50 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s233, i64 0, i64 0)) #11
  ret float %call50

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup23
  %indvars.iv80 = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next81, %for.cond.cleanup23 ]
  %arrayidx11.phi.trans.insert = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0, i64 %indvars.iv80
  %.pre = load float, float* %arrayidx11.phi.trans.insert, align 4, !tbaa !4
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup23
  %call45 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc47 = add nuw nsw i32 %nl.075, 1
  %exitcond84.not = icmp eq i32 %inc47, 39000
  br i1 %exitcond84.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !120

for.cond21.preheader:                             ; preds = %for.body9
  %0 = add nsw i64 %indvars.iv80, -1
  br label %for.body24

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %1 = phi float [ %.pre, %for.cond6.preheader ], [ %add, %for.body9 ]
  %indvars.iv = phi i64 [ 1, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %arrayidx15 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 %indvars.iv, i64 %indvars.iv80
  %2 = load float, float* %arrayidx15, align 4, !tbaa !4
  %add = fadd float %1, %2
  %arrayidx19 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv, i64 %indvars.iv80
  store float %add, float* %arrayidx19, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond21.preheader, label %for.body9, !llvm.loop !121

for.cond.cleanup23:                               ; preds = %for.body24
  %indvars.iv.next81 = add nuw nsw i64 %indvars.iv80, 1
  %exitcond83.not = icmp eq i64 %indvars.iv.next81, 256
  br i1 %exitcond83.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !122

for.body24:                                       ; preds = %for.cond21.preheader, %for.body24
  %indvars.iv77 = phi i64 [ 1, %for.cond21.preheader ], [ %indvars.iv.next78, %for.body24 ]
  %arrayidx29 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv77, i64 %0
  %3 = load float, float* %arrayidx29, align 4, !tbaa !4
  %arrayidx33 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 %indvars.iv77, i64 %indvars.iv80
  %4 = load float, float* %arrayidx33, align 4, !tbaa !4
  %add34 = fadd float %3, %4
  %arrayidx38 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv77, i64 %indvars.iv80
  store float %add34, float* %arrayidx38, align 4, !tbaa !4
  %indvars.iv.next78 = add nuw nsw i64 %indvars.iv77, 1
  %exitcond79.not = icmp eq i64 %indvars.iv.next78, 256
  br i1 %exitcond79.not, label %for.cond.cleanup23, label %for.body24, !llvm.loop !123
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s2233(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2233, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.075 = phi i32 [ 0, %entry ], [ %inc47, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call49 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call50 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2233, i64 0, i64 0)) #11
  ret float %call50

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup23
  %indvars.iv80 = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next81, %for.cond.cleanup23 ]
  %arrayidx11.phi.trans.insert = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0, i64 %indvars.iv80
  %.pre = load float, float* %arrayidx11.phi.trans.insert, align 4, !tbaa !4
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup23
  %call45 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc47 = add nuw nsw i32 %nl.075, 1
  %exitcond84.not = icmp eq i32 %inc47, 39000
  br i1 %exitcond84.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !124

for.cond21.preheader:                             ; preds = %for.body9
  %0 = add nsw i64 %indvars.iv80, -1
  br label %for.body24

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %1 = phi float [ %.pre, %for.cond6.preheader ], [ %add, %for.body9 ]
  %indvars.iv = phi i64 [ 1, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %arrayidx15 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 %indvars.iv, i64 %indvars.iv80
  %2 = load float, float* %arrayidx15, align 4, !tbaa !4
  %add = fadd float %1, %2
  %arrayidx19 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv, i64 %indvars.iv80
  store float %add, float* %arrayidx19, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond21.preheader, label %for.body9, !llvm.loop !125

for.cond.cleanup23:                               ; preds = %for.body24
  %indvars.iv.next81 = add nuw nsw i64 %indvars.iv80, 1
  %exitcond83.not = icmp eq i64 %indvars.iv.next81, 256
  br i1 %exitcond83.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !126

for.body24:                                       ; preds = %for.cond21.preheader, %for.body24
  %indvars.iv77 = phi i64 [ 1, %for.cond21.preheader ], [ %indvars.iv.next78, %for.body24 ]
  %arrayidx29 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %0, i64 %indvars.iv77
  %3 = load float, float* %arrayidx29, align 4, !tbaa !4
  %arrayidx33 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 %indvars.iv80, i64 %indvars.iv77
  %4 = load float, float* %arrayidx33, align 4, !tbaa !4
  %add34 = fadd float %3, %4
  %arrayidx38 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv80, i64 %indvars.iv77
  store float %add34, float* %arrayidx38, align 4, !tbaa !4
  %indvars.iv.next78 = add nuw nsw i64 %indvars.iv77, 1
  %exitcond79.not = icmp eq i64 %indvars.iv.next78, 256
  br i1 %exitcond79.not, label %for.cond.cleanup23, label %for.body24, !llvm.loop !127
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s235(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s235, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.058 = phi i32 [ 0, %entry ], [ %inc35, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call37 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call38 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s235, i64 0, i64 0)) #11
  ret float %call38

for.cond.cleanup4:                                ; preds = %for.cond.cleanup12
  %call33 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc35 = add nuw nsw i32 %nl.058, 1
  %exitcond63.not = icmp eq i32 %inc35, 78000
  br i1 %exitcond63.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !128

for.body5:                                        ; preds = %for.cond2.preheader, %for.cond.cleanup12
  %indvars.iv60 = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next61, %for.cond.cleanup12 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv60
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv60
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv60
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %add = fadd float %2, %mul
  store float %add, float* %arrayidx9, align 4, !tbaa !4
  %arrayidx17.phi.trans.insert = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0, i64 %indvars.iv60
  %.pre = load float, float* %arrayidx17.phi.trans.insert, align 4, !tbaa !4
  br label %for.body13

for.cond.cleanup12:                               ; preds = %for.body13
  %indvars.iv.next61 = add nuw nsw i64 %indvars.iv60, 1
  %exitcond62.not = icmp eq i64 %indvars.iv.next61, 256
  br i1 %exitcond62.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !129

for.body13:                                       ; preds = %for.body5, %for.body13
  %3 = phi float [ %.pre, %for.body5 ], [ %add25, %for.body13 ]
  %indvars.iv = phi i64 [ 1, %for.body5 ], [ %indvars.iv.next, %for.body13 ]
  %arrayidx21 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv, i64 %indvars.iv60
  %4 = load float, float* %arrayidx21, align 4, !tbaa !4
  %mul24 = fmul float %add, %4
  %add25 = fadd float %3, %mul24
  %arrayidx29 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv, i64 %indvars.iv60
  store float %add25, float* %arrayidx29, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup12, label %for.body13, !llvm.loop !130
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s241(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s241, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.043 = phi i32 [ 0, %entry ], [ %inc25, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call27 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call28 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s241, i64 0, i64 0)) #11
  ret float %call28

for.cond.cleanup4:                                ; preds = %for.body5
  %call23 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc25 = add nuw nsw i32 %nl.043, 1
  %exitcond44.not = icmp eq i32 %inc25, 200000
  br i1 %exitcond44.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !131

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul10 = fmul float %mul, %2
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %mul10, float* %arrayidx12, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %arrayidx16 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next
  %3 = load float, float* %arrayidx16, align 4, !tbaa !4
  %mul17 = fmul float %mul10, %3
  %mul20 = fmul float %2, %mul17
  store float %mul20, float* %arrayidx, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !132
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s242(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to %struct.anon.1**
  %1 = load %struct.anon.1*, %struct.anon.1** %0, align 8, !tbaa !43
  %a = getelementptr inbounds %struct.anon.1, %struct.anon.1* %1, i64 0, i32 0
  %2 = load float, float* %a, align 4, !tbaa !133
  %b = getelementptr inbounds %struct.anon.1, %struct.anon.1* %1, i64 0, i32 1
  %3 = load float, float* %b, align 4, !tbaa !135
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s242, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.040 = phi i32 [ 0, %entry ], [ %inc20, %for.cond.cleanup4 ]
  %.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), align 64, !tbaa !4
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call22 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call23 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s242, i64 0, i64 0)) #11
  ret float %call23

for.cond.cleanup4:                                ; preds = %for.body5
  %call18 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc20 = add nuw nsw i32 %nl.040, 1
  %exitcond42.not = icmp eq i32 %inc20, 20000
  br i1 %exitcond42.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !136

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %4 = phi float [ %.pre, %for.cond2.preheader ], [ %add15, %for.body5 ]
  %indvars.iv = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %add = fadd float %2, %4
  %add6 = fadd float %3, %add
  %arrayidx8 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %5 = load float, float* %arrayidx8, align 4, !tbaa !4
  %add9 = fadd float %5, %add6
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %6 = load float, float* %arrayidx11, align 4, !tbaa !4
  %add12 = fadd float %6, %add9
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %7 = load float, float* %arrayidx14, align 4, !tbaa !4
  %add15 = fadd float %7, %add12
  %arrayidx17 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add15, float* %arrayidx17, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !137
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s243(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s243, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.057 = phi i32 [ 0, %entry ], [ %inc35, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call37 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call38 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s243, i64 0, i64 0)) #11
  ret float %call38

for.cond.cleanup4:                                ; preds = %for.body5
  %call33 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc35 = add nuw nsw i32 %nl.057, 1
  %exitcond58.not = icmp eq i32 %inc35, 100000
  br i1 %exitcond58.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !138

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul = fmul float %1, %2
  %add = fadd float %0, %mul
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %arrayidx17 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx17, align 4, !tbaa !4
  %mul18 = fmul float %2, %3
  %add19 = fadd float %add, %mul18
  store float %add19, float* %arrayidx, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %arrayidx26 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next
  %4 = load float, float* %arrayidx26, align 4, !tbaa !4
  %mul29 = fmul float %2, %4
  %add30 = fadd float %add19, %mul29
  store float %add30, float* %arrayidx11, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !139
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s244(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s244, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.054 = phi i32 [ 0, %entry ], [ %inc33, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call35 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call36 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s244, i64 0, i64 0)) #11
  ret float %call36

for.cond.cleanup4:                                ; preds = %for.body5
  %call31 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc33 = add nuw nsw i32 %nl.054, 1
  %exitcond55.not = icmp eq i32 %inc33, 100000
  br i1 %exitcond55.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !140

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul = fmul float %1, %2
  %add = fadd float %0, %mul
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx11, align 4, !tbaa !4
  %add16 = fadd float %0, %1
  store float %add16, float* %arrayidx, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %arrayidx23 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next
  %3 = load float, float* %arrayidx23, align 4, !tbaa !4
  %mul26 = fmul float %2, %3
  %add27 = fadd float %add16, %mul26
  store float %add27, float* %arrayidx23, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !141
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s1244(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1244, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.051 = phi i32 [ 0, %entry ], [ %inc31, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call33 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call34 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1244, i64 0, i64 0)) #11
  ret float %call34

for.cond.cleanup4:                                ; preds = %for.body5
  %call29 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc31 = add nuw nsw i32 %nl.051, 1
  %exitcond52.not = icmp eq i32 %inc31, 100000
  br i1 %exitcond52.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !142

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %1, %1
  %add = fadd float %0, %mul
  %mul14 = fmul float %0, %0
  %add15 = fadd float %mul14, %add
  %add18 = fadd float %1, %add15
  %arrayidx20 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add18, float* %arrayidx20, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %arrayidx25 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next
  %2 = load float, float* %arrayidx25, align 4, !tbaa !4
  %add26 = fadd float %2, %add18
  %arrayidx28 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  store float %add26, float* %arrayidx28, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !143
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s2244(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2244, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.036 = phi i32 [ 0, %entry ], [ %inc20, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call22 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call23 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2244, i64 0, i64 0)) #11
  ret float %call23

for.cond.cleanup4:                                ; preds = %for.body5
  %call18 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc20 = add nuw nsw i32 %nl.036, 1
  %exitcond37.not = icmp eq i32 %inc20, 100000
  br i1 %exitcond37.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !144

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %0, %1
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next
  store float %add, float* %arrayidx10, align 4, !tbaa !4
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx14, align 4, !tbaa !4
  %add15 = fadd float %0, %2
  %arrayidx17 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add15, float* %arrayidx17, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !145
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s251(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s251, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.031 = phi i32 [ 0, %entry ], [ %inc15, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call17 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call18 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s251, i64 0, i64 0)) #11
  ret float %call18

for.cond.cleanup4:                                ; preds = %for.body5
  %call13 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc15 = add nuw nsw i32 %nl.031, 1
  %exitcond32.not = icmp eq i32 %inc15, 400000
  br i1 %exitcond32.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !146

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul = fmul float %1, %2
  %add = fadd float %0, %mul
  %mul10 = fmul float %add, %add
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %mul10, float* %arrayidx12, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !147
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s1251(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1251, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.039 = phi i32 [ 0, %entry ], [ %inc21, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call23 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call24 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1251, i64 0, i64 0)) #11
  ret float %call24

for.cond.cleanup4:                                ; preds = %for.body5
  %call19 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc21 = add nuw nsw i32 %nl.039, 1
  %exitcond40.not = icmp eq i32 %inc21, 400000
  br i1 %exitcond40.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !148

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %0, %1
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx11, align 4, !tbaa !4
  %add12 = fadd float %2, %3
  store float %add12, float* %arrayidx, align 4, !tbaa !4
  %arrayidx16 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx16, align 4, !tbaa !4
  %mul = fmul float %add, %4
  store float %mul, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !149
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s2251(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2251, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.040 = phi i32 [ 0, %entry ], [ %inc21, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call23 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call24 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2251, i64 0, i64 0)) #11
  ret float %call24

for.cond.cleanup4:                                ; preds = %for.body5
  %call19 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc21 = add nuw nsw i32 %nl.040, 1
  %exitcond41.not = icmp eq i32 %inc21, 100000
  br i1 %exitcond41.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !150

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %s.038 = phi float [ 0.000000e+00, %for.cond2.preheader ], [ %add, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %mul = fmul float %s.038, %0
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %mul, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx9, align 4, !tbaa !4
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx11, align 4, !tbaa !4
  %add = fadd float %1, %2
  %arrayidx15 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx15, align 4, !tbaa !4
  %add16 = fadd float %mul, %3
  store float %add16, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !151
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s3251(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s3251, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.045 = phi i32 [ 0, %entry ], [ %inc26, %for.cond.cleanup4 ]
  %.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), align 64, !tbaa !4
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call28 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call29 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s3251, i64 0, i64 0)) #11
  ret float %call29

for.cond.cleanup4:                                ; preds = %for.body5
  %call24 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc26 = add nuw nsw i32 %nl.045, 1
  %exitcond46.not = icmp eq i32 %inc26, 100000
  br i1 %exitcond46.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !152

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %0 = phi float [ %.pre, %for.cond2.preheader ], [ %add, %for.body5 ]
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %1, %2
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next
  store float %add, float* %arrayidx10, align 4, !tbaa !4
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx14, align 4, !tbaa !4
  %mul = fmul float %2, %3
  store float %mul, float* %arrayidx, align 4, !tbaa !4
  %mul21 = fmul float %3, %0
  %arrayidx23 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  store float %mul21, float* %arrayidx23, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !153
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s252(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s252, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.029 = phi i32 [ 0, %entry ], [ %inc12, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call15 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s252, i64 0, i64 0)) #11
  ret float %call15

for.cond.cleanup4:                                ; preds = %for.body5
  %call10 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc12 = add nuw nsw i32 %nl.029, 1
  %exitcond30.not = icmp eq i32 %inc12, 100000
  br i1 %exitcond30.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !154

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %t.027 = phi float [ 0.000000e+00, %for.cond2.preheader ], [ %mul, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %add = fadd float %t.027, %mul
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !155
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s253(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s253, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.041 = phi i32 [ 0, %entry ], [ %inc21, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call23 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call24 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s253, i64 0, i64 0)) #11
  ret float %call24

for.cond.cleanup4:                                ; preds = %for.inc
  %call19 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc21 = add nuw nsw i32 %nl.041, 1
  %exitcond42.not = icmp eq i32 %inc21, 100000
  br i1 %exitcond42.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !156

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %cmp8 = fcmp ogt float %0, %1
  br i1 %cmp8, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body5
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx14, align 4, !tbaa !4
  %mul = fmul float %1, %2
  %sub = fsub float %0, %mul
  %arrayidx16 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx16, align 4, !tbaa !4
  %add = fadd float %3, %sub
  store float %add, float* %arrayidx16, align 4, !tbaa !4
  store float %sub, float* %arrayidx, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %for.body5, %if.then
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !157
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s254(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s254, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.027 = phi i32 [ 0, %entry ], [ %inc12, %for.cond.cleanup4 ]
  %x.0.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 31999), align 4, !tbaa !4
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call15 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s254, i64 0, i64 0)) #11
  ret float %call15

for.cond.cleanup4:                                ; preds = %for.body5
  %call10 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc12 = add nuw nsw i32 %nl.027, 1
  %exitcond28.not = icmp eq i32 %inc12, 400000
  br i1 %exitcond28.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !158

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %x.0 = phi float [ %x.0.pre, %for.cond2.preheader ], [ %0, %for.body5 ]
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %add = fadd float %x.0, %0
  %mul = fmul float %add, 5.000000e-01
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %mul, float* %arrayidx7, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !159
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s255(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s255, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call15 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call16 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s255, i64 0, i64 0)) #11
  ret float %call16

for.body:                                         ; preds = %entry, %for.cond.cleanup4
  %nl.032 = phi i32 [ 0, %entry ], [ %inc13, %for.cond.cleanup4 ]
  %0 = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 31998), align 8, !tbaa !4
  %x.031.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 31999), align 4, !tbaa !4
  br label %for.body5

for.cond.cleanup4:                                ; preds = %for.body5
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc13 = add nuw nsw i32 %nl.032, 1
  %exitcond33.not = icmp eq i32 %inc13, 100000
  br i1 %exitcond33.not, label %for.cond.cleanup, label %for.body, !llvm.loop !160

for.body5:                                        ; preds = %for.body, %for.body5
  %x.031 = phi float [ %x.031.pre, %for.body ], [ %1, %for.body5 ]
  %indvars.iv = phi i64 [ 0, %for.body ], [ %indvars.iv.next, %for.body5 ]
  %y.029 = phi float [ %0, %for.body ], [ %x.031, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %add = fadd float %x.031, %1
  %add6 = fadd float %y.029, %add
  %mul = fmul float %add6, 0x3FD54FDF40000000
  %arrayidx8 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %mul, float* %arrayidx8, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !161
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s256(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s256, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.051 = phi i32 [ 0, %entry ], [ %inc30, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call32 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call33 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s256, i64 0, i64 0)) #11
  ret float %call33

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv53 = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next54, %for.cond.cleanup8 ]
  %.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), align 64, !tbaa !4
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call28 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc30 = add nuw nsw i32 %nl.051, 1
  %exitcond56.not = icmp eq i32 %inc30, 3900
  br i1 %exitcond56.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !162

for.cond.cleanup8:                                ; preds = %for.body9
  %indvars.iv.next54 = add nuw nsw i64 %indvars.iv53, 1
  %exitcond55.not = icmp eq i64 %indvars.iv.next54, 256
  br i1 %exitcond55.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !163

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %0 = phi float [ %.pre, %for.cond6.preheader ], [ %sub10, %for.body9 ]
  %indvars.iv = phi i64 [ 1, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %sub10 = fsub float 1.000000e+00, %0
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %sub10, float* %arrayidx12, align 4, !tbaa !4
  %arrayidx18 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv, i64 %indvars.iv53
  %1 = load float, float* %arrayidx18, align 4, !tbaa !4
  %arrayidx20 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx20, align 4, !tbaa !4
  %mul = fmul float %1, %2
  %add = fadd float %sub10, %mul
  %arrayidx24 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv, i64 %indvars.iv53
  store float %add, float* %arrayidx24, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !164
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s257(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s257, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.054 = phi i32 [ 0, %entry ], [ %inc32, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call34 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call35 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s257, i64 0, i64 0)) #11
  ret float %call35

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv55 = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next56, %for.cond.cleanup8 ]
  %0 = add nsw i64 %indvars.iv55, -1
  %arrayidx13 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %0
  %arrayidx16 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv55
  %.pre = load float, float* %arrayidx13, align 4, !tbaa !4
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call30 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc32 = add nuw nsw i32 %nl.054, 1
  %exitcond59.not = icmp eq i32 %inc32, 3900
  br i1 %exitcond59.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !165

for.cond.cleanup8:                                ; preds = %for.body9
  store float %sub14, float* %arrayidx16, align 4, !tbaa !4
  %indvars.iv.next56 = add nuw nsw i64 %indvars.iv55, 1
  %exitcond58.not = icmp eq i64 %indvars.iv.next56, 256
  br i1 %exitcond58.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !166

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %indvars.iv = phi i64 [ 0, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %arrayidx11 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv, i64 %indvars.iv55
  %1 = load float, float* %arrayidx11, align 4, !tbaa !4
  %sub14 = fsub float %1, %.pre
  %arrayidx22 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv, i64 %indvars.iv55
  %2 = load float, float* %arrayidx22, align 4, !tbaa !4
  %add = fadd float %sub14, %2
  store float %add, float* %arrayidx11, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !167
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s258(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s258, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.048 = phi i32 [ 0, %entry ], [ %inc27, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call29 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call30 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s258, i64 0, i64 0)) #11
  ret float %call30

for.cond.cleanup4:                                ; preds = %for.body5
  %call25 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc27 = add nuw nsw i32 %nl.048, 1
  %exitcond49.not = icmp eq i32 %inc27, 100000
  br i1 %exitcond49.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !168

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %s.046 = phi float [ 0.000000e+00, %for.cond2.preheader ], [ %s.1, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp ogt float %0, 0.000000e+00
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul = fmul float %1, %1
  %s.1 = select i1 %cmp6, float %mul, float %s.046
  %arrayidx13 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx13, align 4, !tbaa !4
  %mul14 = fmul float %s.1, %2
  %add = fadd float %mul14, %1
  %arrayidx18 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx18, align 4, !tbaa !4
  %add19 = fadd float %s.1, 1.000000e+00
  %arrayidx21 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx21, align 4, !tbaa !4
  %mul22 = fmul float %add19, %3
  %arrayidx24 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  store float %mul22, float* %arrayidx24, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !169
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s261(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s261, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.040 = phi i32 [ 0, %entry ], [ %inc21, %for.cond.cleanup4 ]
  %.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), align 64, !tbaa !4
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call23 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call24 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s261, i64 0, i64 0)) #11
  ret float %call24

for.cond.cleanup4:                                ; preds = %for.body5
  %call19 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc21 = add nuw nsw i32 %nl.040, 1
  %exitcond42.not = icmp eq i32 %inc21, 100000
  br i1 %exitcond42.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !170

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %0 = phi float [ %.pre, %for.cond2.preheader ], [ %mul, %for.body5 ]
  %indvars.iv = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %1, %2
  %add10 = fadd float %add, %0
  store float %add10, float* %arrayidx, align 4, !tbaa !4
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx14, align 4, !tbaa !4
  %arrayidx16 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx16, align 4, !tbaa !4
  %mul = fmul float %3, %4
  store float %mul, float* %arrayidx14, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !171
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s271(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s271, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.029 = phi i32 [ 0, %entry ], [ %inc15, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call17 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call18 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s271, i64 0, i64 0)) #11
  ret float %call18

for.cond.cleanup4:                                ; preds = %for.inc
  %call13 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc15 = add nuw nsw i32 %nl.029, 1
  %exitcond30.not = icmp eq i32 %inc15, 400000
  br i1 %exitcond30.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !172

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp ogt float %0, 0.000000e+00
  br i1 %cmp6, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body5
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx10, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx12, align 4, !tbaa !4
  %add = fadd float %2, %mul
  store float %add, float* %arrayidx12, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %for.body5, %if.then
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !173
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s272(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to i32**
  %1 = load i32*, i32** %0, align 8, !tbaa !43
  %2 = load i32, i32* %1, align 4, !tbaa !83
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s272, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  %conv = sitofp i32 %2 to float
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.043 = phi i32 [ 0, %entry ], [ %inc24, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call26 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call27 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s272, i64 0, i64 0)) #11
  ret float %call27

for.cond.cleanup4:                                ; preds = %for.inc
  %call22 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc24 = add nuw nsw i32 %nl.043, 1
  %exitcond44.not = icmp eq i32 %inc24, 100000
  br i1 %exitcond44.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !174

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp ult float %3, %conv
  br i1 %cmp6, label %for.inc, label %if.then

if.then:                                          ; preds = %for.body5
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx9, align 4, !tbaa !4
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %5 = load float, float* %arrayidx11, align 4, !tbaa !4
  %mul = fmul float %4, %5
  %arrayidx13 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %6 = load float, float* %arrayidx13, align 4, !tbaa !4
  %add = fadd float %6, %mul
  store float %add, float* %arrayidx13, align 4, !tbaa !4
  %mul18 = fmul float %4, %4
  %arrayidx20 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %7 = load float, float* %arrayidx20, align 4, !tbaa !4
  %add21 = fadd float %mul18, %7
  store float %add21, float* %arrayidx20, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %for.body5, %if.then
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !175
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s273(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s273, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.051 = phi i32 [ 0, %entry ], [ %inc31, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call33 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call34 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s273, i64 0, i64 0)) #11
  ret float %call34

for.cond.cleanup4:                                ; preds = %if.end
  %call29 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc31 = add nuw nsw i32 %nl.051, 1
  %exitcond52.not = icmp eq i32 %inc31, 100000
  br i1 %exitcond52.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !176

for.body5:                                        ; preds = %for.cond2.preheader, %if.end
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %if.end ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %add = fadd float %2, %mul
  store float %add, float* %arrayidx9, align 4, !tbaa !4
  %cmp12 = fcmp olt float %add, 0.000000e+00
  br i1 %cmp12, label %if.then, label %if.end

if.then:                                          ; preds = %for.body5
  %arrayidx19 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx19, align 4, !tbaa !4
  %add20 = fadd float %mul, %3
  store float %add20, float* %arrayidx19, align 4, !tbaa !4
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body5
  %mul25 = fmul float %0, %add
  %arrayidx27 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx27, align 4, !tbaa !4
  %add28 = fadd float %mul25, %4
  store float %add28, float* %arrayidx27, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !177
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s274(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s274, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.052 = phi i32 [ 0, %entry ], [ %inc31, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call33 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call34 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s274, i64 0, i64 0)) #11
  ret float %call34

for.cond.cleanup4:                                ; preds = %for.inc
  %call29 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc31 = add nuw nsw i32 %nl.052, 1
  %exitcond53.not = icmp eq i32 %inc31, 100000
  br i1 %exitcond53.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !178

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul = fmul float %1, %2
  %add = fadd float %0, %mul
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx11, align 4, !tbaa !4
  %cmp14 = fcmp ogt float %add, 0.000000e+00
  br i1 %cmp14, label %if.then, label %if.else

if.then:                                          ; preds = %for.body5
  %arrayidx18 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx18, align 4, !tbaa !4
  %add19 = fadd float %add, %3
  store float %add19, float* %arrayidx18, align 4, !tbaa !4
  br label %for.inc

if.else:                                          ; preds = %for.body5
  store float %mul, float* %arrayidx11, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %if.then, %if.else
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !179
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s275(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s275, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.054 = phi i32 [ 0, %entry ], [ %inc32, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call34 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call35 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s275, i64 0, i64 0)) #11
  ret float %call35

for.cond.cleanup4:                                ; preds = %for.inc27
  %call30 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc32 = add nuw nsw i32 %nl.054, 1
  %exitcond59.not = icmp eq i32 %inc32, 3900
  br i1 %exitcond59.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !180

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc27
  %indvars.iv56 = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next57, %for.inc27 ]
  %arrayidx = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0, i64 %indvars.iv56
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp ogt float %0, 0.000000e+00
  br i1 %cmp6, label %for.body10, label %for.inc27

for.body10:                                       ; preds = %for.body5, %for.body10
  %1 = phi float [ %add, %for.body10 ], [ %0, %for.body5 ]
  %indvars.iv = phi i64 [ %indvars.iv.next, %for.body10 ], [ 1, %for.body5 ]
  %arrayidx18 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv, i64 %indvars.iv56
  %2 = load float, float* %arrayidx18, align 4, !tbaa !4
  %arrayidx22 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 %indvars.iv, i64 %indvars.iv56
  %3 = load float, float* %arrayidx22, align 4, !tbaa !4
  %mul = fmul float %2, %3
  %add = fadd float %1, %mul
  %arrayidx26 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv, i64 %indvars.iv56
  store float %add, float* %arrayidx26, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.inc27, label %for.body10, !llvm.loop !181

for.inc27:                                        ; preds = %for.body10, %for.body5
  %indvars.iv.next57 = add nuw nsw i64 %indvars.iv56, 1
  %exitcond58.not = icmp eq i64 %indvars.iv.next57, 256
  br i1 %exitcond58.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !182
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s2275(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2275, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.064 = phi i32 [ 0, %entry ], [ %inc39, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call41 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call42 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2275, i64 0, i64 0)) #11
  ret float %call42

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv65 = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next66, %for.cond.cleanup8 ]
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call37 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc39 = add nuw nsw i32 %nl.064, 1
  %exitcond68.not = icmp eq i32 %inc39, 39000
  br i1 %exitcond68.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !183

for.cond.cleanup8:                                ; preds = %for.body9
  %arrayidx25 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv65
  %0 = load float, float* %arrayidx25, align 4, !tbaa !4
  %arrayidx27 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv65
  %1 = load float, float* %arrayidx27, align 4, !tbaa !4
  %arrayidx29 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv65
  %2 = load float, float* %arrayidx29, align 4, !tbaa !4
  %mul30 = fmul float %1, %2
  %add31 = fadd float %0, %mul30
  %arrayidx33 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv65
  store float %add31, float* %arrayidx33, align 4, !tbaa !4
  %indvars.iv.next66 = add nuw nsw i64 %indvars.iv65, 1
  %exitcond67.not = icmp eq i64 %indvars.iv.next66, 256
  br i1 %exitcond67.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !184

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %indvars.iv = phi i64 [ 0, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %arrayidx11 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv, i64 %indvars.iv65
  %3 = load float, float* %arrayidx11, align 4, !tbaa !4
  %arrayidx15 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv, i64 %indvars.iv65
  %4 = load float, float* %arrayidx15, align 4, !tbaa !4
  %arrayidx19 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 %indvars.iv, i64 %indvars.iv65
  %5 = load float, float* %arrayidx19, align 4, !tbaa !4
  %mul = fmul float %4, %5
  %add = fadd float %3, %mul
  store float %add, float* %arrayidx11, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !185
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s276(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s276, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.040 = phi i32 [ 0, %entry ], [ %inc22, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call24 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call25 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s276, i64 0, i64 0)) #11
  ret float %call25

for.cond.cleanup4:                                ; preds = %for.body5
  %call20 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc22 = add nuw nsw i32 %nl.040, 1
  %exitcond41.not = icmp eq i32 %inc22, 400000
  br i1 %exitcond41.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !186

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %cmp6 = icmp ult i64 %indvars.iv, 15999
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %c.sink = select i1 %cmp6, [32000 x float]* @c, [32000 x float]* @d
  %arrayidx8 = getelementptr inbounds [32000 x float], [32000 x float]* %c.sink, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx8, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx10, align 4, !tbaa !4
  %add11 = fadd float %2, %mul
  store float %add11, float* %arrayidx10, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !187
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s277(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s277, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.050 = phi i32 [ 0, %entry ], [ %inc31, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call33 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call34 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s277, i64 0, i64 0)) #11
  ret float %call34

for.cond.cleanup4:                                ; preds = %for.inc
  %call29 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc31 = add nuw nsw i32 %nl.050, 1
  %exitcond52.not = icmp eq i32 %inc31, 100000
  br i1 %exitcond52.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !188

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next.pre-phi, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp ult float %0, 0.000000e+00
  br i1 %cmp6, label %if.end, label %for.body5.for.inc_crit_edge

for.body5.for.inc_crit_edge:                      ; preds = %for.body5
  %.pre54 = add nuw nsw i64 %indvars.iv, 1
  br label %for.inc

if.end:                                           ; preds = %for.body5
  %arrayidx8 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx8, align 4, !tbaa !4
  %cmp9 = fcmp ult float %1, 0.000000e+00
  %arrayidx13 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx13, align 4, !tbaa !4
  %arrayidx15 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx15, align 4, !tbaa !4
  br i1 %cmp9, label %if.end11, label %L30

if.end11:                                         ; preds = %if.end
  %mul = fmul float %2, %3
  %add = fadd float %0, %mul
  store float %add, float* %arrayidx, align 4, !tbaa !4
  br label %L30

L30:                                              ; preds = %if.end, %if.end11
  %arrayidx23 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx23, align 4, !tbaa !4
  %mul24 = fmul float %3, %4
  %add25 = fadd float %2, %mul24
  %5 = add nuw nsw i64 %indvars.iv, 1
  %arrayidx28 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %5
  store float %add25, float* %arrayidx28, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %for.body5.for.inc_crit_edge, %L30
  %indvars.iv.next.pre-phi = phi i64 [ %.pre54, %for.body5.for.inc_crit_edge ], [ %5, %L30 ]
  %exitcond.not = icmp eq i64 %indvars.iv.next.pre-phi, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !189
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s278(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s278, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.061 = phi i32 [ 0, %entry ], [ %inc38, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call40 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call41 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s278, i64 0, i64 0)) #11
  ret float %call41

for.cond.cleanup4:                                ; preds = %L30
  %call36 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc38 = add nuw nsw i32 %nl.061, 1
  %exitcond62.not = icmp eq i32 %inc38, 100000
  br i1 %exitcond62.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !190

for.body5:                                        ; preds = %for.cond2.preheader, %L30
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %L30 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp ogt float %0, 0.000000e+00
  br i1 %cmp6, label %L20, label %if.end

if.end:                                           ; preds = %for.body5
  %arrayidx8 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx8, align 4, !tbaa !4
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx10, align 4, !tbaa !4
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx12, align 4, !tbaa !4
  %mul = fmul float %2, %3
  %add = fsub float %mul, %1
  store float %add, float* %arrayidx8, align 4, !tbaa !4
  %arrayidx29.phi.trans.insert = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %.pre63 = load float, float* %arrayidx29.phi.trans.insert, align 4, !tbaa !4
  br label %L30

L20:                                              ; preds = %for.body5
  %arrayidx16 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx16, align 4, !tbaa !4
  %arrayidx19 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %5 = load float, float* %arrayidx19, align 4, !tbaa !4
  %arrayidx21 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %6 = load float, float* %arrayidx21, align 4, !tbaa !4
  %mul22 = fmul float %5, %6
  %add23 = fsub float %mul22, %4
  store float %add23, float* %arrayidx16, align 4, !tbaa !4
  %arrayidx27.phi.trans.insert = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %.pre = load float, float* %arrayidx27.phi.trans.insert, align 4, !tbaa !4
  br label %L30

L30:                                              ; preds = %L20, %if.end
  %7 = phi float [ %5, %L20 ], [ %2, %if.end ]
  %8 = phi float [ %add23, %L20 ], [ %.pre63, %if.end ]
  %9 = phi float [ %.pre, %L20 ], [ %add, %if.end ]
  %mul32 = fmul float %8, %7
  %add33 = fadd float %9, %mul32
  store float %add33, float* %arrayidx, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !191
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s279(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s279, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.081 = phi i32 [ 0, %entry ], [ %inc53, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call55 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call56 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s279, i64 0, i64 0)) #11
  ret float %call56

for.cond.cleanup4:                                ; preds = %L30
  %call51 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc53 = add nuw nsw i32 %nl.081, 1
  %exitcond82.not = icmp eq i32 %inc53, 50000
  br i1 %exitcond82.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !192

for.body5:                                        ; preds = %for.cond2.preheader, %L30
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %L30 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp ogt float %0, 0.000000e+00
  br i1 %cmp6, label %L20, label %if.end

if.end:                                           ; preds = %for.body5
  %arrayidx8 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx8, align 4, !tbaa !4
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx10, align 4, !tbaa !4
  %mul = fmul float %2, %2
  %add = fsub float %mul, %1
  store float %add, float* %arrayidx8, align 4, !tbaa !4
  %cmp19 = fcmp ugt float %add, %0
  br i1 %cmp19, label %if.end21, label %if.end.L30_crit_edge

if.end.L30_crit_edge:                             ; preds = %if.end
  %arrayidx44.phi.trans.insert = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %.pre83 = load float, float* %arrayidx44.phi.trans.insert, align 4, !tbaa !4
  br label %L30

if.end21:                                         ; preds = %if.end
  %arrayidx25 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx25, align 4, !tbaa !4
  %mul26 = fmul float %2, %3
  %arrayidx28 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx28, align 4, !tbaa !4
  %add29 = fadd float %4, %mul26
  store float %add29, float* %arrayidx28, align 4, !tbaa !4
  br label %L30

L20:                                              ; preds = %for.body5
  %arrayidx31 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %5 = load float, float* %arrayidx31, align 4, !tbaa !4
  %arrayidx34 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %6 = load float, float* %arrayidx34, align 4, !tbaa !4
  %mul37 = fmul float %6, %6
  %add38 = fsub float %mul37, %5
  store float %add38, float* %arrayidx31, align 4, !tbaa !4
  %arrayidx42.phi.trans.insert = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %.pre = load float, float* %arrayidx42.phi.trans.insert, align 4, !tbaa !4
  %arrayidx46.phi.trans.insert = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %.pre84 = load float, float* %arrayidx46.phi.trans.insert, align 4, !tbaa !4
  br label %L30

L30:                                              ; preds = %if.end.L30_crit_edge, %L20, %if.end21
  %7 = phi float [ %2, %if.end.L30_crit_edge ], [ %.pre84, %L20 ], [ %2, %if.end21 ]
  %8 = phi float [ %.pre83, %if.end.L30_crit_edge ], [ %add38, %L20 ], [ %add29, %if.end21 ]
  %9 = phi float [ %add, %if.end.L30_crit_edge ], [ %.pre, %L20 ], [ %add, %if.end21 ]
  %mul47 = fmul float %8, %7
  %add48 = fadd float %9, %mul47
  store float %add48, float* %arrayidx, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !193
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s1279(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1279, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.038 = phi i32 [ 0, %entry ], [ %inc22, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call24 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call25 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1279, i64 0, i64 0)) #11
  ret float %call25

for.cond.cleanup4:                                ; preds = %for.inc
  %call20 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc22 = add nuw nsw i32 %nl.038, 1
  %exitcond39.not = icmp eq i32 %inc22, 100000
  br i1 %exitcond39.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !194

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp olt float %0, 0.000000e+00
  br i1 %cmp6, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body5
  %arrayidx8 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx8, align 4, !tbaa !4
  %cmp11 = fcmp ogt float %1, %0
  br i1 %cmp11, label %if.then12, label %for.inc

if.then12:                                        ; preds = %if.then
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx14, align 4, !tbaa !4
  %arrayidx16 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx16, align 4, !tbaa !4
  %mul = fmul float %2, %3
  %arrayidx18 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx18, align 4, !tbaa !4
  %add = fadd float %4, %mul
  store float %add, float* %arrayidx18, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %for.body5, %if.then12, %if.then
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !195
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s2710(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to i32**
  %1 = load i32*, i32** %0, align 8, !tbaa !43
  %2 = load i32, i32* %1, align 4, !tbaa !83
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2710, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  %cmp33 = icmp sgt i32 %2, 0
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.090 = phi i32 [ 0, %entry ], [ %inc58, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call60 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call61 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2710, i64 0, i64 0)) #11
  ret float %call61

for.cond.cleanup4:                                ; preds = %for.inc
  %call56 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc58 = add nuw nsw i32 %nl.090, 1
  %exitcond91.not = icmp eq i32 %inc58, 50000
  br i1 %exitcond91.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !196

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx7, align 4, !tbaa !4
  %cmp8 = fcmp ogt float %3, %4
  br i1 %cmp8, label %if.then, label %if.else

if.then:                                          ; preds = %for.body5
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %5 = load float, float* %arrayidx12, align 4, !tbaa !4
  %mul = fmul float %4, %5
  %add = fadd float %3, %mul
  store float %add, float* %arrayidx, align 4, !tbaa !4
  %mul19 = fmul float %5, %5
  %arrayidx21 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %6 = load float, float* %arrayidx21, align 4, !tbaa !4
  %add22 = fadd float %mul19, %6
  store float %add22, float* %arrayidx21, align 4, !tbaa !4
  br label %for.inc

if.else:                                          ; preds = %for.body5
  %arrayidx26 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %7 = load float, float* %arrayidx26, align 4, !tbaa !4
  %mul29 = fmul float %7, %7
  %add30 = fadd float %3, %mul29
  store float %add30, float* %arrayidx7, align 4, !tbaa !4
  br i1 %cmp33, label %if.then35, label %if.else46

if.then35:                                        ; preds = %if.else
  %arrayidx39 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %8 = load float, float* %arrayidx39, align 4, !tbaa !4
  %mul42 = fmul float %8, %8
  %add43 = fadd float %3, %mul42
  %arrayidx45 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  store float %add43, float* %arrayidx45, align 4, !tbaa !4
  br label %for.inc

if.else46:                                        ; preds = %if.else
  %arrayidx53 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %9 = load float, float* %arrayidx53, align 4, !tbaa !4
  %add54 = fadd float %mul29, %9
  store float %add54, float* %arrayidx53, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %if.then, %if.else46, %if.then35
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !197
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s2711(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2711, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.029 = phi i32 [ 0, %entry ], [ %inc15, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call17 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call18 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2711, i64 0, i64 0)) #11
  ret float %call18

for.cond.cleanup4:                                ; preds = %for.inc
  %call13 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc15 = add nuw nsw i32 %nl.029, 1
  %exitcond30.not = icmp eq i32 %inc15, 400000
  br i1 %exitcond30.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !198

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp une float %0, 0.000000e+00
  br i1 %cmp6, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body5
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx10, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx12, align 4, !tbaa !4
  %add = fadd float %2, %mul
  store float %add, float* %arrayidx12, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %for.body5, %if.then
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !199
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s2712(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2712, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.033 = phi i32 [ 0, %entry ], [ %inc17, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call19 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call20 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2712, i64 0, i64 0)) #11
  ret float %call20

for.cond.cleanup4:                                ; preds = %for.inc
  %call15 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc17 = add nuw nsw i32 %nl.033, 1
  %exitcond34.not = icmp eq i32 %inc17, 400000
  br i1 %exitcond34.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !200

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %cmp8 = fcmp ogt float %0, %1
  br i1 %cmp8, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body5
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx12, align 4, !tbaa !4
  %mul = fmul float %1, %2
  %add = fadd float %0, %mul
  store float %add, float* %arrayidx, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %for.body5, %if.then
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !201
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s281(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s281, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.035 = phi i32 [ 0, %entry ], [ %inc18, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call20 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call21 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s281, i64 0, i64 0)) #11
  ret float %call21

for.cond.cleanup4:                                ; preds = %for.body5
  %call16 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc18 = add nuw nsw i32 %nl.035, 1
  %exitcond37.not = icmp eq i32 %inc18, 100000
  br i1 %exitcond37.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !202

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %0 = sub nuw nsw i64 31999, %indvars.iv
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %0
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx8 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx8, align 4, !tbaa !4
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx10, align 4, !tbaa !4
  %mul = fmul float %2, %3
  %add = fadd float %1, %mul
  %sub11 = fadd float %add, -1.000000e+00
  %arrayidx13 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %sub11, float* %arrayidx13, align 4, !tbaa !4
  store float %add, float* %arrayidx8, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !203
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s1281(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1281, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.041 = phi i32 [ 0, %entry ], [ %inc22, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call24 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call25 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1281, i64 0, i64 0)) #11
  ret float %call25

for.cond.cleanup4:                                ; preds = %for.body5
  %call20 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc22 = add nuw nsw i32 %nl.041, 1
  %exitcond42.not = icmp eq i32 %inc22, 400000
  br i1 %exitcond42.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !204

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx11, align 4, !tbaa !4
  %mul12 = fmul float %2, %3
  %add = fadd float %mul, %mul12
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx14, align 4, !tbaa !4
  %add15 = fadd float %4, %add
  %sub = fadd float %add15, -1.000000e+00
  store float %sub, float* %arrayidx9, align 4, !tbaa !4
  store float %add15, float* %arrayidx, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !205
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s291(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s291, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.027 = phi i32 [ 0, %entry ], [ %inc12, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call15 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s291, i64 0, i64 0)) #11
  ret float %call15

for.cond.cleanup4:                                ; preds = %for.body5
  %call10 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc12 = add nuw nsw i32 %nl.027, 1
  %exitcond28.not = icmp eq i32 %inc12, 200000
  br i1 %exitcond28.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !206

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %im1.025 = phi i64 [ 31999, %for.cond2.preheader ], [ %indvars.iv, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %idxprom6 = and i64 %im1.025, 4294967295
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %idxprom6
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %0, %1
  %mul = fmul float %add, 5.000000e-01
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %mul, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !207
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s292(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s292, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.034 = phi i32 [ 0, %entry ], [ %inc15, %for.cond.cleanup4 ]
  %.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 31998), align 8, !tbaa !4
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call17 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call18 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s292, i64 0, i64 0)) #11
  ret float %call18

for.cond.cleanup4:                                ; preds = %for.body5
  %call13 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc15 = add nuw nsw i32 %nl.034, 1
  %exitcond35.not = icmp eq i32 %inc15, 100000
  br i1 %exitcond35.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !208

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %0 = phi float [ %.pre, %for.cond2.preheader ], [ %2, %for.body5 ]
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %im1.031 = phi i64 [ 31999, %for.cond2.preheader ], [ %indvars.iv, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %idxprom6 = and i64 %im1.031, 4294967295
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %idxprom6
  %2 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %1, %2
  %add10 = fadd float %add, %0
  %mul = fmul float %add10, 0x3FD54FDF40000000
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %mul, float* %arrayidx12, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !209
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s293(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s293, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.019 = phi i32 [ 0, %entry ], [ %inc8, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call10 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call11 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s293, i64 0, i64 0)) #11
  ret float %call11

for.cond.cleanup4:                                ; preds = %for.body5
  %call6 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc8 = add nuw nsw i32 %nl.019, 1
  %exitcond20.not = icmp eq i32 %inc8, 400000
  br i1 %exitcond20.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !210

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %0 = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), align 64, !tbaa !4
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %0, float* %arrayidx, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !211
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s2101(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2101, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.034 = phi i32 [ 0, %entry ], [ %inc18, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call20 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call21 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2101, i64 0, i64 0)) #11
  ret float %call21

for.cond.cleanup4:                                ; preds = %for.body5
  %call16 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc18 = add nuw nsw i32 %nl.034, 1
  %exitcond35.not = icmp eq i32 %inc18, 1000000
  br i1 %exitcond35.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !212

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx7 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv, i64 %indvars.iv
  %0 = load float, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx11 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 %indvars.iv, i64 %indvars.iv
  %1 = load float, float* %arrayidx11, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx15 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv, i64 %indvars.iv
  %2 = load float, float* %arrayidx15, align 4, !tbaa !4
  %add = fadd float %2, %mul
  store float %add, float* %arrayidx15, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !213
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s2102(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2102, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.038 = phi i32 [ 0, %entry ], [ %inc21, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call23 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call24 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2102, i64 0, i64 0)) #11
  ret float %call24

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv39 = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next40, %for.cond.cleanup8 ]
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call19 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc21 = add nuw nsw i32 %nl.038, 1
  %exitcond42.not = icmp eq i32 %inc21, 39000
  br i1 %exitcond42.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !214

for.cond.cleanup8:                                ; preds = %for.body9
  %arrayidx15 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv39, i64 %indvars.iv39
  store float 1.000000e+00, float* %arrayidx15, align 4, !tbaa !4
  %indvars.iv.next40 = add nuw nsw i64 %indvars.iv39, 1
  %exitcond41.not = icmp eq i64 %indvars.iv.next40, 256
  br i1 %exitcond41.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !215

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %indvars.iv = phi i64 [ 0, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %arrayidx11 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv, i64 %indvars.iv39
  store float 0.000000e+00, float* %arrayidx11, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !216
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s2111(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2111, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.046 = phi i32 [ 0, %entry ], [ %inc27, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call29 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call30 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s2111, i64 0, i64 0)) #11
  ret float %call30

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv48 = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next49, %for.cond.cleanup8 ]
  %0 = add nsw i64 %indvars.iv48, -1
  %arrayidx11.phi.trans.insert = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv48, i64 0
  %.pre = load float, float* %arrayidx11.phi.trans.insert, align 64, !tbaa !4
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call25 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc27 = add nuw nsw i32 %nl.046, 1
  %exitcond52.not = icmp eq i32 %inc27, 39000
  br i1 %exitcond52.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !217

for.cond.cleanup8:                                ; preds = %for.body9
  %indvars.iv.next49 = add nuw nsw i64 %indvars.iv48, 1
  %exitcond51.not = icmp eq i64 %indvars.iv.next49, 256
  br i1 %exitcond51.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !218

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %1 = phi float [ %.pre, %for.cond6.preheader ], [ %conv17, %for.body9 ]
  %indvars.iv = phi i64 [ 1, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %arrayidx16 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %0, i64 %indvars.iv
  %2 = load float, float* %arrayidx16, align 4, !tbaa !4
  %add = fadd float %1, %2
  %conv = fpext float %add to double
  %div = fdiv double %conv, 1.900000e+00
  %conv17 = fptrunc double %div to float
  %arrayidx21 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv48, i64 %indvars.iv
  store float %conv17, float* %arrayidx21, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !219
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s311(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s311, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.022 = phi i32 [ 0, %entry ], [ %inc8, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call10 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call11 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s311, i64 0, i64 0)) #11
  ret float %call11

for.cond.cleanup4:                                ; preds = %for.body5
  %call6 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %add) #11
  %inc8 = add nuw nsw i32 %nl.022, 1
  %exitcond23.not = icmp eq i32 %inc8, 1000000
  br i1 %exitcond23.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !220

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %sum.020 = phi float [ 0.000000e+00, %for.cond2.preheader ], [ %add, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %add = fadd float %sum.020, %0
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !221
}

; Function Attrs: norecurse nounwind optsize readonly uwtable
define dso_local float @test(float* nocapture readonly %A) local_unnamed_addr #5 {
entry:
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.body
  ret float %add

for.body:                                         ; preds = %entry, %for.body
  %indvars.iv = phi i64 [ 0, %entry ], [ %indvars.iv.next, %for.body ]
  %s.06 = phi float [ 0.000000e+00, %entry ], [ %add, %for.body ]
  %arrayidx = getelementptr inbounds float, float* %A, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %add = fadd float %s.06, %0
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 4
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.body, !llvm.loop !222
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s31111(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @__func__.s31111, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.body.i.preheader

for.body.i.preheader:                             ; preds = %entry, %test.exit39
  %nl.088 = phi i32 [ 0, %entry ], [ %inc, %test.exit39 ]
  br label %for.body.i

for.cond.cleanup:                                 ; preds = %test.exit39
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call18 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call19 = tail call float @calc_checksum(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @__func__.s31111, i64 0, i64 0)) #11
  ret float %call19

for.body.i:                                       ; preds = %for.body.i.preheader, %for.body.i
  %indvars.iv.i = phi i64 [ %indvars.iv.next.i, %for.body.i ], [ 0, %for.body.i.preheader ]
  %s.06.i = phi float [ %add.i, %for.body.i ], [ 0.000000e+00, %for.body.i.preheader ]
  %arrayidx.i = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.i
  %0 = load float, float* %arrayidx.i, align 4, !tbaa !4
  %add.i = fadd float %s.06.i, %0
  %indvars.iv.next.i = add nuw nsw i64 %indvars.iv.i, 1
  %exitcond.not.i = icmp eq i64 %indvars.iv.next.i, 4
  br i1 %exitcond.not.i, label %for.body.i86, label %for.body.i, !llvm.loop !222

for.body.i86:                                     ; preds = %for.body.i, %for.body.i86
  %indvars.iv.i80 = phi i64 [ %indvars.iv.next.i84, %for.body.i86 ], [ 0, %for.body.i ]
  %s.06.i81 = phi float [ %add.i83, %for.body.i86 ], [ 0.000000e+00, %for.body.i ]
  %arrayidx.i82 = getelementptr inbounds float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 4), i64 %indvars.iv.i80
  %1 = load float, float* %arrayidx.i82, align 4, !tbaa !4
  %add.i83 = fadd float %s.06.i81, %1
  %indvars.iv.next.i84 = add nuw nsw i64 %indvars.iv.i80, 1
  %exitcond.not.i85 = icmp eq i64 %indvars.iv.next.i84, 4
  br i1 %exitcond.not.i85, label %test.exit87, label %for.body.i86, !llvm.loop !222

test.exit87:                                      ; preds = %for.body.i86
  %add = fadd float %add.i, 0.000000e+00
  br label %for.body.i78

for.body.i78:                                     ; preds = %for.body.i78, %test.exit87
  %indvars.iv.i72 = phi i64 [ 0, %test.exit87 ], [ %indvars.iv.next.i76, %for.body.i78 ]
  %s.06.i73 = phi float [ 0.000000e+00, %test.exit87 ], [ %add.i75, %for.body.i78 ]
  %arrayidx.i74 = getelementptr inbounds float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 8), i64 %indvars.iv.i72
  %2 = load float, float* %arrayidx.i74, align 4, !tbaa !4
  %add.i75 = fadd float %s.06.i73, %2
  %indvars.iv.next.i76 = add nuw nsw i64 %indvars.iv.i72, 1
  %exitcond.not.i77 = icmp eq i64 %indvars.iv.next.i76, 4
  br i1 %exitcond.not.i77, label %test.exit79, label %for.body.i78, !llvm.loop !222

test.exit79:                                      ; preds = %for.body.i78
  %add4 = fadd float %add, %add.i83
  br label %for.body.i70

for.body.i70:                                     ; preds = %for.body.i70, %test.exit79
  %indvars.iv.i64 = phi i64 [ 0, %test.exit79 ], [ %indvars.iv.next.i68, %for.body.i70 ]
  %s.06.i65 = phi float [ 0.000000e+00, %test.exit79 ], [ %add.i67, %for.body.i70 ]
  %arrayidx.i66 = getelementptr inbounds float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 12), i64 %indvars.iv.i64
  %3 = load float, float* %arrayidx.i66, align 4, !tbaa !4
  %add.i67 = fadd float %s.06.i65, %3
  %indvars.iv.next.i68 = add nuw nsw i64 %indvars.iv.i64, 1
  %exitcond.not.i69 = icmp eq i64 %indvars.iv.next.i68, 4
  br i1 %exitcond.not.i69, label %test.exit71, label %for.body.i70, !llvm.loop !222

test.exit71:                                      ; preds = %for.body.i70
  %add6 = fadd float %add4, %add.i75
  br label %for.body.i62

for.body.i62:                                     ; preds = %for.body.i62, %test.exit71
  %indvars.iv.i56 = phi i64 [ 0, %test.exit71 ], [ %indvars.iv.next.i60, %for.body.i62 ]
  %s.06.i57 = phi float [ 0.000000e+00, %test.exit71 ], [ %add.i59, %for.body.i62 ]
  %arrayidx.i58 = getelementptr inbounds float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 16), i64 %indvars.iv.i56
  %4 = load float, float* %arrayidx.i58, align 4, !tbaa !4
  %add.i59 = fadd float %s.06.i57, %4
  %indvars.iv.next.i60 = add nuw nsw i64 %indvars.iv.i56, 1
  %exitcond.not.i61 = icmp eq i64 %indvars.iv.next.i60, 4
  br i1 %exitcond.not.i61, label %test.exit63, label %for.body.i62, !llvm.loop !222

test.exit63:                                      ; preds = %for.body.i62
  %add8 = fadd float %add6, %add.i67
  br label %for.body.i54

for.body.i54:                                     ; preds = %for.body.i54, %test.exit63
  %indvars.iv.i48 = phi i64 [ 0, %test.exit63 ], [ %indvars.iv.next.i52, %for.body.i54 ]
  %s.06.i49 = phi float [ 0.000000e+00, %test.exit63 ], [ %add.i51, %for.body.i54 ]
  %arrayidx.i50 = getelementptr inbounds float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 20), i64 %indvars.iv.i48
  %5 = load float, float* %arrayidx.i50, align 4, !tbaa !4
  %add.i51 = fadd float %s.06.i49, %5
  %indvars.iv.next.i52 = add nuw nsw i64 %indvars.iv.i48, 1
  %exitcond.not.i53 = icmp eq i64 %indvars.iv.next.i52, 4
  br i1 %exitcond.not.i53, label %test.exit55, label %for.body.i54, !llvm.loop !222

test.exit55:                                      ; preds = %for.body.i54
  %add10 = fadd float %add8, %add.i59
  br label %for.body.i46

for.body.i46:                                     ; preds = %for.body.i46, %test.exit55
  %indvars.iv.i40 = phi i64 [ 0, %test.exit55 ], [ %indvars.iv.next.i44, %for.body.i46 ]
  %s.06.i41 = phi float [ 0.000000e+00, %test.exit55 ], [ %add.i43, %for.body.i46 ]
  %arrayidx.i42 = getelementptr inbounds float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 24), i64 %indvars.iv.i40
  %6 = load float, float* %arrayidx.i42, align 4, !tbaa !4
  %add.i43 = fadd float %s.06.i41, %6
  %indvars.iv.next.i44 = add nuw nsw i64 %indvars.iv.i40, 1
  %exitcond.not.i45 = icmp eq i64 %indvars.iv.next.i44, 4
  br i1 %exitcond.not.i45, label %test.exit47, label %for.body.i46, !llvm.loop !222

test.exit47:                                      ; preds = %for.body.i46
  %add12 = fadd float %add10, %add.i51
  br label %for.body.i38

for.body.i38:                                     ; preds = %for.body.i38, %test.exit47
  %indvars.iv.i32 = phi i64 [ 0, %test.exit47 ], [ %indvars.iv.next.i36, %for.body.i38 ]
  %s.06.i33 = phi float [ 0.000000e+00, %test.exit47 ], [ %add.i35, %for.body.i38 ]
  %arrayidx.i34 = getelementptr inbounds float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 28), i64 %indvars.iv.i32
  %7 = load float, float* %arrayidx.i34, align 4, !tbaa !4
  %add.i35 = fadd float %s.06.i33, %7
  %indvars.iv.next.i36 = add nuw nsw i64 %indvars.iv.i32, 1
  %exitcond.not.i37 = icmp eq i64 %indvars.iv.next.i36, 4
  br i1 %exitcond.not.i37, label %test.exit39, label %for.body.i38, !llvm.loop !222

test.exit39:                                      ; preds = %for.body.i38
  %add14 = fadd float %add12, %add.i43
  %add16 = fadd float %add14, %add.i35
  %call17 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %add16) #11
  %inc = add nuw nsw i32 %nl.088, 1
  %exitcond.not = icmp eq i32 %inc, 200000000
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.body.i.preheader, !llvm.loop !223
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s312(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s312, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.022 = phi i32 [ 0, %entry ], [ %inc8, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call10 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  ret float %mul

for.cond.cleanup4:                                ; preds = %for.body5
  %call6 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %mul) #11
  %inc8 = add nuw nsw i32 %nl.022, 1
  %exitcond23.not = icmp eq i32 %inc8, 1000000
  br i1 %exitcond23.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !224

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %prod.120 = phi float [ 1.000000e+00, %for.cond2.preheader ], [ %mul, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %mul = fmul float %prod.120, %0
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !225
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s313(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s313, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.025 = phi i32 [ 0, %entry ], [ %inc10, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call12 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  ret float %add

for.cond.cleanup4:                                ; preds = %for.body5
  %call8 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %add) #11
  %inc10 = add nuw nsw i32 %nl.025, 1
  %exitcond26.not = icmp eq i32 %inc10, 500000
  br i1 %exitcond26.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !226

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %dot.123 = phi float [ 0.000000e+00, %for.cond2.preheader ], [ %add, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %add = fadd float %dot.123, %mul
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !227
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s314(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s314, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call13 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  ret float %x.2

for.body:                                         ; preds = %entry, %for.cond.cleanup4
  %nl.026 = phi i32 [ 0, %entry ], [ %inc11, %for.cond.cleanup4 ]
  %0 = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), align 64, !tbaa !4
  br label %for.body5.for.body5_crit_edge

for.cond.cleanup4:                                ; preds = %for.body5.for.body5_crit_edge
  %call9 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %x.2) #11
  %inc11 = add nuw nsw i32 %nl.026, 1
  %exitcond27.not = icmp eq i32 %inc11, 500000
  br i1 %exitcond27.not, label %for.cond.cleanup, label %for.body, !llvm.loop !228

for.body5.for.body5_crit_edge:                    ; preds = %for.body, %for.body5.for.body5_crit_edge
  %indvars.iv.next30 = phi i64 [ 1, %for.body ], [ %indvars.iv.next, %for.body5.for.body5_crit_edge ]
  %x.229 = phi float [ %0, %for.body ], [ %x.2, %for.body5.for.body5_crit_edge ]
  %arrayidx.phi.trans.insert = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next30
  %.pre = load float, float* %arrayidx.phi.trans.insert, align 4, !tbaa !4
  %cmp6 = fcmp ogt float %.pre, %x.229
  %x.2 = select i1 %cmp6, float %.pre, float %x.229
  %indvars.iv.next = add nuw nsw i64 %indvars.iv.next30, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5.for.body5_crit_edge, !llvm.loop !229
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s315(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s315, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.body

for.body:                                         ; preds = %entry, %for.body
  %indvars.iv55 = phi i64 [ 0, %entry ], [ %indvars.iv.next56, %for.body ]
  %0 = trunc i64 %indvars.iv55 to i32
  %1 = mul i32 %0, 7
  %rem = urem i32 %1, 32000
  %conv = sitofp i32 %rem to float
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv55
  store float %conv, float* %arrayidx, align 4, !tbaa !4
  %indvars.iv.next56 = add nuw nsw i64 %indvars.iv55, 1
  %exitcond58.not = icmp eq i64 %indvars.iv.next56, 32000
  br i1 %exitcond58.not, label %for.body6, label %for.body, !llvm.loop !230

for.cond.cleanup5:                                ; preds = %for.cond.cleanup11
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call27 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %add30 = fadd float %add, 1.000000e+00
  ret float %add30

for.body6:                                        ; preds = %for.body, %for.cond.cleanup11
  %nl.052 = phi i32 [ %inc25, %for.cond.cleanup11 ], [ 0, %for.body ]
  %2 = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), align 64, !tbaa !4
  br label %for.body12.for.body12_crit_edge

for.cond.cleanup11:                               ; preds = %for.body12.for.body12_crit_edge
  %conv22 = sitofp i32 %index.2 to float
  %add = fadd float %x.2, %conv22
  %call23 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %add) #11
  %inc25 = add nuw nsw i32 %nl.052, 1
  %exitcond54.not = icmp eq i32 %inc25, 100000
  br i1 %exitcond54.not, label %for.cond.cleanup5, label %for.body6, !llvm.loop !231

for.body12.for.body12_crit_edge:                  ; preds = %for.body6, %for.body12.for.body12_crit_edge
  %indvars.iv.next62 = phi i64 [ 1, %for.body6 ], [ %indvars.iv.next, %for.body12.for.body12_crit_edge ]
  %index.261 = phi i32 [ 0, %for.body6 ], [ %index.2, %for.body12.for.body12_crit_edge ]
  %x.260 = phi float [ %2, %for.body6 ], [ %x.2, %for.body12.for.body12_crit_edge ]
  %arrayidx14.phi.trans.insert = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next62
  %.pre = load float, float* %arrayidx14.phi.trans.insert, align 4, !tbaa !4
  %cmp15 = fcmp ogt float %.pre, %x.260
  %x.2 = select i1 %cmp15, float %.pre, float %x.260
  %3 = trunc i64 %indvars.iv.next62 to i32
  %index.2 = select i1 %cmp15, i32 %3, i32 %index.261
  %indvars.iv.next = add nuw nsw i64 %indvars.iv.next62, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup11, label %for.body12.for.body12_crit_edge, !llvm.loop !232
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s316(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s316, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call13 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  ret float %x.2

for.body:                                         ; preds = %entry, %for.cond.cleanup4
  %nl.026 = phi i32 [ 0, %entry ], [ %inc11, %for.cond.cleanup4 ]
  %0 = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), align 64, !tbaa !4
  br label %for.body5

for.cond.cleanup4:                                ; preds = %for.body5
  %call9 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %x.2) #11
  %inc11 = add nuw nsw i32 %nl.026, 1
  %exitcond27.not = icmp eq i32 %inc11, 500000
  br i1 %exitcond27.not, label %for.cond.cleanup, label %for.body, !llvm.loop !233

for.body5:                                        ; preds = %for.body, %for.body5
  %indvars.iv = phi i64 [ 1, %for.body ], [ %indvars.iv.next, %for.body5 ]
  %x.124 = phi float [ %0, %for.body ], [ %x.2, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp olt float %1, %x.124
  %x.2 = select i1 %cmp6, float %1, float %x.124
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !234
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s317(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s317, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.021 = phi i32 [ 0, %entry ], [ %inc8, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call10 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  ret float %mul

for.cond.cleanup4:                                ; preds = %for.body5
  %call6 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %mul) #11
  %inc8 = add nuw nsw i32 %nl.021, 1
  %exitcond22.not = icmp eq i32 %inc8, 500000
  br i1 %exitcond22.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !235

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %i.020 = phi i32 [ 0, %for.cond2.preheader ], [ %inc, %for.body5 ]
  %q.119 = phi float [ 1.000000e+00, %for.cond2.preheader ], [ %mul, %for.body5 ]
  %mul = fmul float %q.119, 0x3FEFAE1480000000
  %inc = add nuw nsw i32 %i.020, 1
  %exitcond.not = icmp eq i32 %inc, 16000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !236
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s318(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to i32**
  %1 = load i32*, i32** %0, align 8, !tbaa !43
  %2 = load i32, i32* %1, align 4, !tbaa !83
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s318, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  %3 = sext i32 %2 to i64
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call16 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %add19 = fadd float %add11, 1.000000e+00
  ret float %add19

for.body:                                         ; preds = %entry, %for.cond.cleanup4
  %nl.043 = phi i32 [ 0, %entry ], [ %inc14, %for.cond.cleanup4 ]
  %4 = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), align 64, !tbaa !4
  %5 = tail call float @llvm.fabs.f32(float %4)
  br label %for.body5

for.cond.cleanup4:                                ; preds = %for.body5
  %conv = sitofp i32 %index.2 to float
  %add11 = fadd float %max.2, %conv
  %call12 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %add11) #11
  %inc14 = add nuw nsw i32 %nl.043, 1
  %exitcond44.not = icmp eq i32 %inc14, 50000
  br i1 %exitcond44.not, label %for.cond.cleanup, label %for.body, !llvm.loop !237

for.body5:                                        ; preds = %for.body, %for.body5
  %indvars.iv = phi i64 [ %3, %for.body ], [ %indvars.iv.next, %for.body5 ]
  %i.042 = phi i32 [ 1, %for.body ], [ %inc10, %for.body5 ]
  %max.140 = phi float [ %5, %for.body ], [ %max.2, %for.body5 ]
  %index.139 = phi i32 [ 0, %for.body ], [ %index.2, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %6 = load float, float* %arrayidx, align 4, !tbaa !4
  %7 = tail call float @llvm.fabs.f32(float %6)
  %cmp6 = fcmp ugt float %7, %max.140
  %index.2 = select i1 %cmp6, i32 %i.042, i32 %index.139
  %max.2 = select i1 %cmp6, float %7, float %max.140
  %indvars.iv.next = add i64 %indvars.iv, %3
  %inc10 = add nuw nsw i32 %i.042, 1
  %exitcond.not = icmp eq i32 %inc10, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !238
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare float @llvm.fabs.f32(float) #6

; Function Attrs: nounwind optsize uwtable
define dso_local float @s319(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s319, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.047 = phi i32 [ 0, %entry ], [ %inc25, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call27 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  ret float %add22

for.cond.cleanup4:                                ; preds = %for.body5
  %call23 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %add22) #11
  %inc25 = add nuw nsw i32 %nl.047, 1
  %exitcond48.not = icmp eq i32 %inc25, 200000
  br i1 %exitcond48.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !239

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %sum.145 = phi float [ 0.000000e+00, %for.cond2.preheader ], [ %add22, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %0, %1
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx9, align 4, !tbaa !4
  %add12 = fadd float %sum.145, %add
  %arrayidx16 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx16, align 4, !tbaa !4
  %add17 = fadd float %0, %2
  %arrayidx19 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  store float %add17, float* %arrayidx19, align 4, !tbaa !4
  %add22 = fadd float %add12, %add17
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !240
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s3110(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s3110, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call26 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %add29 = fadd float %add, 1.000000e+00
  %add31 = fadd float %add29, %conv20
  %add32 = fadd float %add31, 1.000000e+00
  ret float %add32

for.body:                                         ; preds = %entry, %for.cond.cleanup4
  %nl.062 = phi i32 [ 0, %entry ], [ %inc24, %for.cond.cleanup4 ]
  %0 = load float, float* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0, i64 0), align 64, !tbaa !4
  br label %for.cond6.preheader

for.cond6.preheader:                              ; preds = %for.body, %for.cond.cleanup8
  %indvars.iv63 = phi i64 [ 0, %for.body ], [ %indvars.iv.next64, %for.cond.cleanup8 ]
  %xindex.160 = phi i32 [ 0, %for.body ], [ %xindex.3, %for.cond.cleanup8 ]
  %max.159 = phi float [ %0, %for.body ], [ %max.3, %for.cond.cleanup8 ]
  %yindex.158 = phi i32 [ 0, %for.body ], [ %yindex.3, %for.cond.cleanup8 ]
  %1 = trunc i64 %indvars.iv63 to i32
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %conv = sitofp i32 %xindex.3 to float
  %add = fadd float %max.3, %conv
  %conv20 = sitofp i32 %yindex.3 to float
  %add21 = fadd float %add, %conv20
  %call22 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %add21) #11
  %inc24 = add nuw nsw i32 %nl.062, 1
  %exitcond66.not = icmp eq i32 %inc24, 39000
  br i1 %exitcond66.not, label %for.cond.cleanup, label %for.body, !llvm.loop !241

for.cond.cleanup8:                                ; preds = %for.body9
  %indvars.iv.next64 = add nuw nsw i64 %indvars.iv63, 1
  %exitcond65.not = icmp eq i64 %indvars.iv.next64, 256
  br i1 %exitcond65.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !242

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %indvars.iv = phi i64 [ 0, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %xindex.256 = phi i32 [ %xindex.160, %for.cond6.preheader ], [ %xindex.3, %for.body9 ]
  %max.255 = phi float [ %max.159, %for.cond6.preheader ], [ %max.3, %for.body9 ]
  %yindex.254 = phi i32 [ %yindex.158, %for.cond6.preheader ], [ %yindex.3, %for.body9 ]
  %arrayidx11 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv63, i64 %indvars.iv
  %2 = load float, float* %arrayidx11, align 4, !tbaa !4
  %cmp12 = fcmp ogt float %2, %max.255
  %3 = trunc i64 %indvars.iv to i32
  %yindex.3 = select i1 %cmp12, i32 %3, i32 %yindex.254
  %max.3 = select i1 %cmp12, float %2, float %max.255
  %xindex.3 = select i1 %cmp12, i32 %1, i32 %xindex.256
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !243
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s13110(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @__func__.s13110, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call26 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %add29 = fadd float %add, 1.000000e+00
  %add31 = fadd float %add29, %conv20
  %add32 = fadd float %add31, 1.000000e+00
  ret float %add32

for.body:                                         ; preds = %entry, %for.cond.cleanup4
  %nl.062 = phi i32 [ 0, %entry ], [ %inc24, %for.cond.cleanup4 ]
  %0 = load float, float* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0, i64 0), align 64, !tbaa !4
  br label %for.cond6.preheader

for.cond6.preheader:                              ; preds = %for.body, %for.cond.cleanup8
  %indvars.iv63 = phi i64 [ 0, %for.body ], [ %indvars.iv.next64, %for.cond.cleanup8 ]
  %xindex.160 = phi i32 [ 0, %for.body ], [ %xindex.3, %for.cond.cleanup8 ]
  %max.159 = phi float [ %0, %for.body ], [ %max.3, %for.cond.cleanup8 ]
  %yindex.158 = phi i32 [ 0, %for.body ], [ %yindex.3, %for.cond.cleanup8 ]
  %1 = trunc i64 %indvars.iv63 to i32
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %conv = sitofp i32 %xindex.3 to float
  %add = fadd float %max.3, %conv
  %conv20 = sitofp i32 %yindex.3 to float
  %add21 = fadd float %add, %conv20
  %call22 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %add21) #11
  %inc24 = add nuw nsw i32 %nl.062, 1
  %exitcond66.not = icmp eq i32 %inc24, 39000
  br i1 %exitcond66.not, label %for.cond.cleanup, label %for.body, !llvm.loop !244

for.cond.cleanup8:                                ; preds = %for.body9
  %indvars.iv.next64 = add nuw nsw i64 %indvars.iv63, 1
  %exitcond65.not = icmp eq i64 %indvars.iv.next64, 256
  br i1 %exitcond65.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !245

for.body9:                                        ; preds = %for.cond6.preheader, %for.body9
  %indvars.iv = phi i64 [ 0, %for.cond6.preheader ], [ %indvars.iv.next, %for.body9 ]
  %xindex.256 = phi i32 [ %xindex.160, %for.cond6.preheader ], [ %xindex.3, %for.body9 ]
  %max.255 = phi float [ %max.159, %for.cond6.preheader ], [ %max.3, %for.body9 ]
  %yindex.254 = phi i32 [ %yindex.158, %for.cond6.preheader ], [ %yindex.3, %for.body9 ]
  %arrayidx11 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv63, i64 %indvars.iv
  %2 = load float, float* %arrayidx11, align 4, !tbaa !4
  %cmp12 = fcmp ogt float %2, %max.255
  %3 = trunc i64 %indvars.iv to i32
  %yindex.3 = select i1 %cmp12, i32 %3, i32 %yindex.254
  %max.3 = select i1 %cmp12, float %2, float %max.255
  %xindex.3 = select i1 %cmp12, i32 %1, i32 %xindex.256
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !246
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s3111(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s3111, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.026 = phi i32 [ 0, %entry ], [ %inc11, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call13 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  ret float %sum.2

for.cond.cleanup4:                                ; preds = %for.body5
  %call9 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %sum.2) #11
  %inc11 = add nuw nsw i32 %nl.026, 1
  %exitcond27.not = icmp eq i32 %inc11, 50000
  br i1 %exitcond27.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !247

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %sum.124 = phi float [ 0.000000e+00, %for.cond2.preheader ], [ %sum.2, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp ogt float %0, 0.000000e+00
  %add = fadd float %sum.124, %0
  %sum.2 = select i1 %cmp6, float %add, float %sum.124
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !248
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s3112(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s3112, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.026 = phi i32 [ 0, %entry ], [ %inc10, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call12 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  ret float %add

for.cond.cleanup4:                                ; preds = %for.body5
  %call8 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %add) #11
  %inc10 = add nuw nsw i32 %nl.026, 1
  %exitcond27.not = icmp eq i32 %inc10, 100000
  br i1 %exitcond27.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !249

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %sum.124 = phi float [ 0.000000e+00, %for.cond2.preheader ], [ %add, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %add = fadd float %sum.124, %0
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx7, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !250
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s3113(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s3113, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call13 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  ret float %max.2

for.body:                                         ; preds = %entry, %for.cond.cleanup4
  %nl.026 = phi i32 [ 0, %entry ], [ %inc11, %for.cond.cleanup4 ]
  %0 = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), align 64, !tbaa !4
  %1 = tail call float @llvm.fabs.f32(float %0)
  %2 = tail call float @llvm.fabs.f32(float %0)
  %cmp628 = fcmp ogt float %2, %1
  %max.229 = select i1 %cmp628, float %2, float %1
  br label %for.body5.for.body5_crit_edge

for.cond.cleanup4:                                ; preds = %for.body5.for.body5_crit_edge
  %call9 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %max.2) #11
  %inc11 = add nuw nsw i32 %nl.026, 1
  %exitcond27.not = icmp eq i32 %inc11, 400000
  br i1 %exitcond27.not, label %for.cond.cleanup, label %for.body, !llvm.loop !251

for.body5.for.body5_crit_edge:                    ; preds = %for.body, %for.body5.for.body5_crit_edge
  %indvars.iv.next31 = phi i64 [ 1, %for.body ], [ %indvars.iv.next, %for.body5.for.body5_crit_edge ]
  %max.230 = phi float [ %max.229, %for.body ], [ %max.2, %for.body5.for.body5_crit_edge ]
  %arrayidx.phi.trans.insert = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv.next31
  %.pre = load float, float* %arrayidx.phi.trans.insert, align 4, !tbaa !4
  %3 = tail call float @llvm.fabs.f32(float %.pre)
  %cmp6 = fcmp ogt float %3, %max.230
  %max.2 = select i1 %cmp6, float %3, float %max.230
  %indvars.iv.next = add nuw nsw i64 %indvars.iv.next31, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5.for.body5_crit_edge, !llvm.loop !252
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s321(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s321, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.025 = phi i32 [ 0, %entry ], [ %inc12, %for.cond.cleanup4 ]
  %.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), align 64, !tbaa !4
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call15 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s321, i64 0, i64 0)) #11
  ret float %call15

for.cond.cleanup4:                                ; preds = %for.body5
  %call10 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc12 = add nuw nsw i32 %nl.025, 1
  %exitcond27.not = icmp eq i32 %inc12, 100000
  br i1 %exitcond27.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !253

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %0 = phi float [ %.pre, %for.cond2.preheader ], [ %add, %for.body5 ]
  %indvars.iv = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %add = fadd float %2, %mul
  store float %add, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !254
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s322(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s322, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.037 = phi i32 [ 0, %entry ], [ %inc21, %for.cond.cleanup4 ]
  %.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 1), align 4, !tbaa !4
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call23 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call24 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s322, i64 0, i64 0)) #11
  ret float %call24

for.cond.cleanup4:                                ; preds = %for.body5
  %call19 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc21 = add nuw nsw i32 %nl.037, 1
  %exitcond40.not = icmp eq i32 %inc21, 50000
  br i1 %exitcond40.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !255

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %0 = phi float [ %.pre, %for.cond2.preheader ], [ %add16, %for.body5 ]
  %indvars.iv = phi i64 [ 2, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul = fmul float %0, %2
  %add = fadd float %1, %mul
  %3 = add nsw i64 %indvars.iv, -2
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %3
  %4 = load float, float* %arrayidx12, align 4, !tbaa !4
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %5 = load float, float* %arrayidx14, align 4, !tbaa !4
  %mul15 = fmul float %4, %5
  %add16 = fadd float %add, %mul15
  store float %add16, float* %arrayidx, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !256
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s323(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s323, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.042 = phi i32 [ 0, %entry ], [ %inc24, %for.cond.cleanup4 ]
  %.pre = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), align 64, !tbaa !4
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call26 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call27 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s323, i64 0, i64 0)) #11
  ret float %call27

for.cond.cleanup4:                                ; preds = %for.body5
  %call22 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc24 = add nuw nsw i32 %nl.042, 1
  %exitcond44.not = icmp eq i32 %inc24, 50000
  br i1 %exitcond44.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !257

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %0 = phi float [ %.pre, %for.cond2.preheader ], [ %add19, %for.body5 ]
  %indvars.iv = phi i64 [ 1, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul = fmul float %1, %2
  %add = fadd float %0, %mul
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx11, align 4, !tbaa !4
  %arrayidx17 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx17, align 4, !tbaa !4
  %mul18 = fmul float %1, %3
  %add19 = fadd float %add, %mul18
  %arrayidx21 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  store float %add19, float* %arrayidx21, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !258
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s331(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s331, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.025 = phi i32 [ 0, %entry ], [ %inc9, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call11 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %add = add nsw i32 %j.2, 1
  %conv12 = sitofp i32 %add to float
  ret float %conv12

for.cond.cleanup4:                                ; preds = %for.body5
  %conv = sitofp i32 %j.2 to float
  %call7 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %conv) #11
  %inc9 = add nuw nsw i32 %nl.025, 1
  %exitcond26.not = icmp eq i32 %inc9, 100000
  br i1 %exitcond26.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !259

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %j.123 = phi i32 [ -1, %for.cond2.preheader ], [ %j.2, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp olt float %0, 0.000000e+00
  %1 = trunc i64 %indvars.iv to i32
  %j.2 = select i1 %cmp6, i32 %1, i32 %j.123
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !260
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s332(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to i32**
  %1 = load i32*, i32** %0, align 8, !tbaa !43
  %2 = load i32, i32* %1, align 4, !tbaa !83
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s332, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  %conv = sitofp i32 %2 to float
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %L20
  %nl.036 = phi i32 [ 0, %entry ], [ %inc13, %L20 ]
  br label %for.body5

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp ogt float %3, %conv
  br i1 %cmp6, label %L20.split.loop.exit38, label %for.inc

for.inc:                                          ; preds = %for.body5
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %L20, label %for.body5, !llvm.loop !261

L20.split.loop.exit38:                            ; preds = %for.body5
  %4 = trunc i64 %indvars.iv to i32
  br label %L20

L20:                                              ; preds = %for.inc, %L20.split.loop.exit38
  %value.1 = phi float [ %3, %L20.split.loop.exit38 ], [ -1.000000e+00, %for.inc ]
  %index.0 = phi i32 [ %4, %L20.split.loop.exit38 ], [ -2, %for.inc ]
  %conv10 = sitofp i32 %index.0 to float
  %add = fadd float %value.1, %conv10
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %add) #11
  %inc13 = add nuw nsw i32 %nl.036, 1
  %exitcond37.not = icmp eq i32 %inc13, 100000
  br i1 %exitcond37.not, label %cleanup14, label %for.cond2.preheader, !llvm.loop !262

cleanup14:                                        ; preds = %L20
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call16 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  ret float %value.1
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s341(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s341, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.029 = phi i32 [ 0, %entry ], [ %inc14, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call16 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call17 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s341, i64 0, i64 0)) #11
  ret float %call17

for.cond.cleanup4:                                ; preds = %for.inc
  %call12 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc14 = add nuw nsw i32 %nl.029, 1
  %exitcond30.not = icmp eq i32 %inc14, 100000
  br i1 %exitcond30.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !263

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %j.027 = phi i32 [ -1, %for.cond2.preheader ], [ %j.1, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp ogt float %0, 0.000000e+00
  br i1 %cmp6, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body5
  %inc = add nsw i32 %j.027, 1
  %idxprom9 = sext i32 %inc to i64
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %idxprom9
  store float %0, float* %arrayidx10, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %for.body5, %if.then
  %j.1 = phi i32 [ %inc, %if.then ], [ %j.027, %for.body5 ]
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !264
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s342(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s342, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.029 = phi i32 [ 0, %entry ], [ %inc14, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call16 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call17 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s342, i64 0, i64 0)) #11
  ret float %call17

for.cond.cleanup4:                                ; preds = %for.inc
  %call12 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc14 = add nuw nsw i32 %nl.029, 1
  %exitcond30.not = icmp eq i32 %inc14, 100000
  br i1 %exitcond30.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !265

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %j.027 = phi i32 [ -1, %for.cond2.preheader ], [ %j.1, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp ogt float %0, 0.000000e+00
  br i1 %cmp6, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body5
  %inc = add nsw i32 %j.027, 1
  %idxprom7 = sext i32 %inc to i64
  %arrayidx8 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %idxprom7
  %1 = load float, float* %arrayidx8, align 4, !tbaa !4
  store float %1, float* %arrayidx, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %for.body5, %if.then
  %j.1 = phi i32 [ %inc, %if.then ], [ %j.027, %for.body5 ]
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !266
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s343(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s343, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.046 = phi i32 [ 0, %entry ], [ %inc25, %for.cond.cleanup4 ]
  br label %for.cond6.preheader

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call27 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call28 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s343, i64 0, i64 0)) #11
  ret float %call28

for.cond6.preheader:                              ; preds = %for.cond2.preheader, %for.cond.cleanup8
  %indvars.iv47 = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next48, %for.cond.cleanup8 ]
  %k.044 = phi i32 [ -1, %for.cond2.preheader ], [ %k.2, %for.cond.cleanup8 ]
  br label %for.body9

for.cond.cleanup4:                                ; preds = %for.cond.cleanup8
  %call23 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc25 = add nuw nsw i32 %nl.046, 1
  %exitcond50.not = icmp eq i32 %inc25, 3900
  br i1 %exitcond50.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !267

for.cond.cleanup8:                                ; preds = %for.inc
  %indvars.iv.next48 = add nuw nsw i64 %indvars.iv47, 1
  %exitcond49.not = icmp eq i64 %indvars.iv.next48, 256
  br i1 %exitcond49.not, label %for.cond.cleanup4, label %for.cond6.preheader, !llvm.loop !268

for.body9:                                        ; preds = %for.cond6.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond6.preheader ], [ %indvars.iv.next, %for.inc ]
  %k.142 = phi i32 [ %k.044, %for.cond6.preheader ], [ %k.2, %for.inc ]
  %arrayidx11 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 %indvars.iv, i64 %indvars.iv47
  %0 = load float, float* %arrayidx11, align 4, !tbaa !4
  %cmp12 = fcmp ogt float %0, 0.000000e+00
  br i1 %cmp12, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body9
  %inc = add nsw i32 %k.142, 1
  %arrayidx16 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %indvars.iv, i64 %indvars.iv47
  %1 = load float, float* %arrayidx16, align 4, !tbaa !4
  %idxprom17 = sext i32 %inc to i64
  %arrayidx18 = getelementptr inbounds [65536 x float], [65536 x float]* @flat_2d_array, i64 0, i64 %idxprom17
  store float %1, float* %arrayidx18, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %for.body9, %if.then
  %k.2 = phi i32 [ %inc, %if.then ], [ %k.142, %for.body9 ]
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup8, label %for.body9, !llvm.loop !269
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s351(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s351, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  %0 = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), align 64, !tbaa !4
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.067 = phi i32 [ 0, %entry ], [ %inc, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call44 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call45 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s351, i64 0, i64 0)) #11
  ret float %call45

for.cond.cleanup4:                                ; preds = %for.body5
  %call41 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc = add nuw nsw i32 %nl.067, 1
  %exitcond.not = icmp eq i32 %inc, 800000
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !270

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %2, %mul
  store float %add, float* %arrayidx7, align 4, !tbaa !4
  %3 = add nuw nsw i64 %indvars.iv, 1
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %3
  %4 = load float, float* %arrayidx10, align 4, !tbaa !4
  %mul11 = fmul float %0, %4
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %3
  %5 = load float, float* %arrayidx14, align 4, !tbaa !4
  %add15 = fadd float %5, %mul11
  store float %add15, float* %arrayidx14, align 4, !tbaa !4
  %6 = add nuw nsw i64 %indvars.iv, 2
  %arrayidx18 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %6
  %7 = load float, float* %arrayidx18, align 4, !tbaa !4
  %mul19 = fmul float %0, %7
  %arrayidx22 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %6
  %8 = load float, float* %arrayidx22, align 4, !tbaa !4
  %add23 = fadd float %8, %mul19
  store float %add23, float* %arrayidx22, align 4, !tbaa !4
  %9 = add nuw nsw i64 %indvars.iv, 3
  %arrayidx26 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %9
  %10 = load float, float* %arrayidx26, align 4, !tbaa !4
  %mul27 = fmul float %0, %10
  %arrayidx30 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %9
  %11 = load float, float* %arrayidx30, align 4, !tbaa !4
  %add31 = fadd float %11, %mul27
  store float %add31, float* %arrayidx30, align 4, !tbaa !4
  %12 = add nuw nsw i64 %indvars.iv, 4
  %arrayidx34 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %12
  %13 = load float, float* %arrayidx34, align 4, !tbaa !4
  %mul35 = fmul float %0, %13
  %arrayidx38 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %12
  %14 = load float, float* %arrayidx38, align 4, !tbaa !4
  %add39 = fadd float %14, %mul35
  store float %add39, float* %arrayidx38, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 5
  %cmp3 = icmp ult i64 %indvars.iv, 31995
  br i1 %cmp3, label %for.body5, label %for.cond.cleanup4, !llvm.loop !271
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s1351(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1351, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.029 = phi i32 [ 0, %entry ], [ %inc10, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call12 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call13 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1351, i64 0, i64 0)) #11
  ret float %call13

for.cond.cleanup4:                                ; preds = %for.body5
  %call8 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc10 = add nuw nsw i32 %nl.029, 1
  %exitcond30.not = icmp eq i32 %inc10, 800000
  br i1 %exitcond30.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !272

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %i.028 = phi i32 [ 0, %for.cond2.preheader ], [ %inc, %for.body5 ]
  %C.027 = phi float* [ getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), %for.cond2.preheader ], [ %incdec.ptr7, %for.body5 ]
  %B.026 = phi float* [ getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), %for.cond2.preheader ], [ %incdec.ptr6, %for.body5 ]
  %A.025 = phi float* [ getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), %for.cond2.preheader ], [ %incdec.ptr, %for.body5 ]
  %0 = load float, float* %B.026, align 4, !tbaa !4
  %1 = load float, float* %C.027, align 4, !tbaa !4
  %add = fadd float %0, %1
  store float %add, float* %A.025, align 4, !tbaa !4
  %incdec.ptr = getelementptr inbounds float, float* %A.025, i64 1
  %incdec.ptr6 = getelementptr inbounds float, float* %B.026, i64 1
  %incdec.ptr7 = getelementptr inbounds float, float* %C.027, i64 1
  %inc = add nuw nsw i32 %i.028, 1
  %exitcond.not = icmp eq i32 %inc, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !273
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s352(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s352, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.065 = phi i32 [ 0, %entry ], [ %inc, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call44 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  ret float %add39

for.cond.cleanup4:                                ; preds = %for.body5
  %call41 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %add39) #11
  %inc = add nuw nsw i32 %nl.065, 1
  %exitcond.not = icmp eq i32 %inc, 800000
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !274

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %dot.163 = phi float [ 0.000000e+00, %for.cond2.preheader ], [ %add39, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %add = fadd float %dot.163, %mul
  %2 = add nuw nsw i64 %indvars.iv, 1
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %2
  %3 = load float, float* %arrayidx10, align 4, !tbaa !4
  %arrayidx13 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %2
  %4 = load float, float* %arrayidx13, align 4, !tbaa !4
  %mul14 = fmul float %3, %4
  %add15 = fadd float %add, %mul14
  %5 = add nuw nsw i64 %indvars.iv, 2
  %arrayidx18 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %5
  %6 = load float, float* %arrayidx18, align 4, !tbaa !4
  %arrayidx21 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %5
  %7 = load float, float* %arrayidx21, align 4, !tbaa !4
  %mul22 = fmul float %6, %7
  %add23 = fadd float %add15, %mul22
  %8 = add nuw nsw i64 %indvars.iv, 3
  %arrayidx26 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %8
  %9 = load float, float* %arrayidx26, align 4, !tbaa !4
  %arrayidx29 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %8
  %10 = load float, float* %arrayidx29, align 4, !tbaa !4
  %mul30 = fmul float %9, %10
  %add31 = fadd float %add23, %mul30
  %11 = add nuw nsw i64 %indvars.iv, 4
  %arrayidx34 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %11
  %12 = load float, float* %arrayidx34, align 4, !tbaa !4
  %arrayidx37 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %11
  %13 = load float, float* %arrayidx37, align 4, !tbaa !4
  %mul38 = fmul float %12, %13
  %add39 = fadd float %add31, %mul38
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 5
  %cmp3 = icmp ult i64 %indvars.iv, 31995
  br i1 %cmp3, label %for.body5, label %for.cond.cleanup4, !llvm.loop !275
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s353(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to i32**
  %1 = load i32*, i32** %0, align 8, !tbaa !43
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s353, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  %2 = load float, float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), align 64, !tbaa !4
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.083 = phi i32 [ 0, %entry ], [ %inc, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call54 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call55 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s353, i64 0, i64 0)) #11
  ret float %call55

for.cond.cleanup4:                                ; preds = %for.body5
  %call51 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc = add nuw nsw i32 %nl.083, 1
  %exitcond.not = icmp eq i32 %inc, 100000
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !276

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds i32, i32* %1, i64 %indvars.iv
  %3 = load i32, i32* %arrayidx, align 4, !tbaa !83
  %idxprom6 = sext i32 %3 to i64
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %idxprom6
  %4 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %2, %4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %5 = load float, float* %arrayidx9, align 4, !tbaa !4
  %add = fadd float %5, %mul
  store float %add, float* %arrayidx9, align 4, !tbaa !4
  %6 = add nuw nsw i64 %indvars.iv, 1
  %arrayidx12 = getelementptr inbounds i32, i32* %1, i64 %6
  %7 = load i32, i32* %arrayidx12, align 4, !tbaa !83
  %idxprom13 = sext i32 %7 to i64
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %idxprom13
  %8 = load float, float* %arrayidx14, align 4, !tbaa !4
  %mul15 = fmul float %2, %8
  %arrayidx18 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %6
  %9 = load float, float* %arrayidx18, align 4, !tbaa !4
  %add19 = fadd float %9, %mul15
  store float %add19, float* %arrayidx18, align 4, !tbaa !4
  %10 = add nuw nsw i64 %indvars.iv, 2
  %arrayidx22 = getelementptr inbounds i32, i32* %1, i64 %10
  %11 = load i32, i32* %arrayidx22, align 4, !tbaa !83
  %idxprom23 = sext i32 %11 to i64
  %arrayidx24 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %idxprom23
  %12 = load float, float* %arrayidx24, align 4, !tbaa !4
  %mul25 = fmul float %2, %12
  %arrayidx28 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %10
  %13 = load float, float* %arrayidx28, align 4, !tbaa !4
  %add29 = fadd float %13, %mul25
  store float %add29, float* %arrayidx28, align 4, !tbaa !4
  %14 = add nuw nsw i64 %indvars.iv, 3
  %arrayidx32 = getelementptr inbounds i32, i32* %1, i64 %14
  %15 = load i32, i32* %arrayidx32, align 4, !tbaa !83
  %idxprom33 = sext i32 %15 to i64
  %arrayidx34 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %idxprom33
  %16 = load float, float* %arrayidx34, align 4, !tbaa !4
  %mul35 = fmul float %2, %16
  %arrayidx38 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %14
  %17 = load float, float* %arrayidx38, align 4, !tbaa !4
  %add39 = fadd float %17, %mul35
  store float %add39, float* %arrayidx38, align 4, !tbaa !4
  %18 = add nuw nsw i64 %indvars.iv, 4
  %arrayidx42 = getelementptr inbounds i32, i32* %1, i64 %18
  %19 = load i32, i32* %arrayidx42, align 4, !tbaa !83
  %idxprom43 = sext i32 %19 to i64
  %arrayidx44 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %idxprom43
  %20 = load float, float* %arrayidx44, align 4, !tbaa !4
  %mul45 = fmul float %2, %20
  %arrayidx48 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %18
  %21 = load float, float* %arrayidx48, align 4, !tbaa !4
  %add49 = fadd float %21, %mul45
  store float %add49, float* %arrayidx48, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 5
  %cmp3 = icmp ult i64 %indvars.iv, 31995
  br i1 %cmp3, label %for.body5, label %for.cond.cleanup4, !llvm.loop !277
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s421(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s421, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  store float* getelementptr inbounds ([65536 x float], [65536 x float]* @flat_2d_array, i64 0, i64 0), float** @xx, align 8, !tbaa !278
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call15 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call16 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s421, i64 0, i64 0)) #11
  ret float %call16

for.body:                                         ; preds = %for.cond.cleanup4.for.body_crit_edge, %entry
  %0 = phi float* [ getelementptr inbounds ([65536 x float], [65536 x float]* @flat_2d_array, i64 0, i64 0), %entry ], [ %.pre, %for.cond.cleanup4.for.body_crit_edge ]
  %nl.026 = phi i32 [ 0, %entry ], [ %inc13, %for.cond.cleanup4.for.body_crit_edge ]
  store float* %0, float** @yy, align 8, !tbaa !278
  br label %for.body5

for.cond.cleanup4:                                ; preds = %for.body5
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 1.000000e+00) #11
  %inc13 = add nuw nsw i32 %nl.026, 1
  %exitcond27.not = icmp eq i32 %inc13, 400000
  br i1 %exitcond27.not, label %for.cond.cleanup, label %for.cond.cleanup4.for.body_crit_edge, !llvm.loop !279

for.cond.cleanup4.for.body_crit_edge:             ; preds = %for.cond.cleanup4
  %.pre = load float*, float** @xx, align 8, !tbaa !278
  br label %for.body

for.body5:                                        ; preds = %for.body, %for.body5
  %indvars.iv = phi i64 [ 0, %for.body ], [ %indvars.iv.next, %for.body5 ]
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %arrayidx = getelementptr inbounds float, float* %0, i64 %indvars.iv.next
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add8 = fadd float %1, %2
  %arrayidx10 = getelementptr inbounds float, float* %0, i64 %indvars.iv
  store float %add8, float* %arrayidx10, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !280
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s1421(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1421, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  store float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 16000), float** @xx, align 8, !tbaa !278
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %for.cond.cleanup4.for.cond2.preheader_crit_edge, %entry
  %0 = phi float* [ getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 16000), %entry ], [ %.pre, %for.cond.cleanup4.for.cond2.preheader_crit_edge ]
  %nl.025 = phi i32 [ 0, %entry ], [ %inc12, %for.cond.cleanup4.for.cond2.preheader_crit_edge ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call15 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s1421, i64 0, i64 0)) #11
  ret float %call15

for.cond.cleanup4:                                ; preds = %for.body5
  %call10 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 1.000000e+00) #11
  %inc12 = add nuw nsw i32 %nl.025, 1
  %exitcond26.not = icmp eq i32 %inc12, 800000
  br i1 %exitcond26.not, label %for.cond.cleanup, label %for.cond.cleanup4.for.cond2.preheader_crit_edge, !llvm.loop !281

for.cond.cleanup4.for.cond2.preheader_crit_edge:  ; preds = %for.cond.cleanup4
  %.pre = load float*, float** @xx, align 8, !tbaa !278
  br label %for.cond2.preheader

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds float, float* %0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %1, %2
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 16000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !282
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s422(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s422, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  store float* getelementptr inbounds ([65536 x float], [65536 x float]* @flat_2d_array, i64 0, i64 4), float** @xx, align 8, !tbaa !278
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %for.cond.cleanup4.for.cond2.preheader_crit_edge, %entry
  %0 = phi float* [ getelementptr inbounds ([65536 x float], [65536 x float]* @flat_2d_array, i64 0, i64 4), %entry ], [ %.pre, %for.cond.cleanup4.for.cond2.preheader_crit_edge ]
  %nl.026 = phi i32 [ 0, %entry ], [ %inc13, %for.cond.cleanup4.for.cond2.preheader_crit_edge ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call15 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call16 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s422, i64 0, i64 0)) #11
  ret float %call16

for.cond.cleanup4:                                ; preds = %for.body5
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc13 = add nuw nsw i32 %nl.026, 1
  %exitcond28.not = icmp eq i32 %inc13, 800000
  br i1 %exitcond28.not, label %for.cond.cleanup, label %for.cond.cleanup4.for.cond2.preheader_crit_edge, !llvm.loop !283

for.cond.cleanup4.for.cond2.preheader_crit_edge:  ; preds = %for.cond.cleanup4
  %.pre = load float*, float** @xx, align 8, !tbaa !278
  br label %for.cond2.preheader

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %1 = add nuw nsw i64 %indvars.iv, 8
  %arrayidx = getelementptr inbounds [65536 x float], [65536 x float]* @flat_2d_array, i64 0, i64 %1
  %2 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add8 = fadd float %2, %3
  %arrayidx10 = getelementptr inbounds float, float* %0, i64 %indvars.iv
  store float %add8, float* %arrayidx10, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !284
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s423(%struct.args_t* nocapture %func_args) #0 {
entry:
  store float* getelementptr inbounds ([65536 x float], [65536 x float]* @flat_2d_array, i64 0, i64 64), float** @xx, align 8, !tbaa !278
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s423, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.027 = phi i32 [ 0, %entry ], [ %inc13, %for.cond.cleanup4 ]
  %0 = load float*, float** @xx, align 8, !tbaa !278
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call15 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call16 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s423, i64 0, i64 0)) #11
  ret float %call16

for.cond.cleanup4:                                ; preds = %for.body5
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 1.000000e+00) #11
  %inc13 = add nuw nsw i32 %nl.027, 1
  %exitcond28.not = icmp eq i32 %inc13, 400000
  br i1 %exitcond28.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !285

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds float, float* %0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %1, %2
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %arrayidx10 = getelementptr inbounds [65536 x float], [65536 x float]* @flat_2d_array, i64 0, i64 %indvars.iv.next
  store float %add, float* %arrayidx10, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !286
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s424(%struct.args_t* nocapture %func_args) #0 {
entry:
  store float* getelementptr inbounds ([65536 x float], [65536 x float]* @flat_2d_array, i64 0, i64 63), float** @xx, align 8, !tbaa !278
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s424, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.027 = phi i32 [ 0, %entry ], [ %inc13, %for.cond.cleanup4 ]
  %0 = load float*, float** @xx, align 8, !tbaa !278
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call15 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call16 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s424, i64 0, i64 0)) #11
  ret float %call16

for.cond.cleanup4:                                ; preds = %for.body5
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 1.000000e+00) #11
  %inc13 = add nuw nsw i32 %nl.027, 1
  %exitcond28.not = icmp eq i32 %inc13, 400000
  br i1 %exitcond28.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !287

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [65536 x float], [65536 x float]* @flat_2d_array, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %1, %2
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %arrayidx10 = getelementptr inbounds float, float* %0, i64 %indvars.iv.next
  store float %add, float* %arrayidx10, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 31999
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !288
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s431(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s431, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.029 = phi i32 [ 0, %entry ], [ %inc13, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call15 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call16 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s431, i64 0, i64 0)) #11
  ret float %call16

for.cond.cleanup4:                                ; preds = %for.body5
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc13 = add nuw nsw i32 %nl.029, 1
  %exitcond30.not = icmp eq i32 %inc13, 1000000
  br i1 %exitcond30.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !289

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add8 = fadd float %0, %1
  store float %add8, float* %arrayidx, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !290
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s441(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s441, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.058 = phi i32 [ 0, %entry ], [ %inc37, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call39 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call40 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s441, i64 0, i64 0)) #11
  ret float %call40

for.cond.cleanup4:                                ; preds = %for.inc
  %call35 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc37 = add nuw nsw i32 %nl.058, 1
  %exitcond59.not = icmp eq i32 %inc37, 100000
  br i1 %exitcond59.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !291

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp olt float %0, 0.000000e+00
  br i1 %cmp6, label %if.then, label %if.else

if.then:                                          ; preds = %for.body5
  %arrayidx8 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx8, align 4, !tbaa !4
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx10, align 4, !tbaa !4
  %mul = fmul float %1, %2
  br label %for.inc

if.else:                                          ; preds = %for.body5
  %cmp15 = fcmp oeq float %0, 0.000000e+00
  br i1 %cmp15, label %if.then16, label %if.else25

if.then16:                                        ; preds = %if.else
  %arrayidx18 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx18, align 4, !tbaa !4
  %mul21 = fmul float %3, %3
  br label %for.inc

if.else25:                                        ; preds = %if.else
  %arrayidx27 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx27, align 4, !tbaa !4
  %mul30 = fmul float %4, %4
  br label %for.inc

for.inc:                                          ; preds = %if.then, %if.else25, %if.then16
  %mul.sink = phi float [ %mul, %if.then ], [ %mul30, %if.else25 ], [ %mul21, %if.then16 ]
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %5 = load float, float* %arrayidx12, align 4, !tbaa !4
  %add = fadd float %5, %mul.sink
  store float %add, float* %arrayidx12, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !292
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s442(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s442, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.064 = phi i32 [ 0, %entry ], [ %inc41, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call43 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call44 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s442, i64 0, i64 0)) #11
  ret float %call44

for.cond.cleanup4:                                ; preds = %for.inc
  %call39 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc41 = add nuw nsw i32 %nl.064, 1
  %exitcond65.not = icmp eq i32 %inc41, 50000
  br i1 %exitcond65.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !293

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x i32], [32000 x i32]* @indx, i64 0, i64 %indvars.iv
  %0 = load i32, i32* %arrayidx, align 4, !tbaa !83
  switch i32 %0, label %L15 [
    i32 4, label %L40
    i32 2, label %L20
    i32 3, label %L30
  ]

L15:                                              ; preds = %for.body5
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx10, align 4, !tbaa !4
  %mul = fmul float %1, %1
  br label %for.inc

L20:                                              ; preds = %for.body5
  %arrayidx16 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx16, align 4, !tbaa !4
  %mul19 = fmul float %2, %2
  br label %for.inc

L30:                                              ; preds = %for.body5
  %arrayidx24 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx24, align 4, !tbaa !4
  %mul27 = fmul float %3, %3
  br label %for.inc

L40:                                              ; preds = %for.body5
  %arrayidx32 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx32, align 4, !tbaa !4
  %mul35 = fmul float %4, %4
  br label %for.inc

for.inc:                                          ; preds = %L15, %L20, %L30, %L40
  %mul.sink = phi float [ %mul, %L15 ], [ %mul19, %L20 ], [ %mul27, %L30 ], [ %mul35, %L40 ]
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %5 = load float, float* %arrayidx14, align 4, !tbaa !4
  %add = fadd float %5, %mul.sink
  store float %add, float* %arrayidx14, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !294
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s443(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s443, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.040 = phi i32 [ 0, %entry ], [ %inc23, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call25 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call26 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s443, i64 0, i64 0)) #11
  ret float %call26

for.cond.cleanup4:                                ; preds = %for.inc
  %call21 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc23 = add nuw nsw i32 %nl.040, 1
  %exitcond41.not = icmp eq i32 %inc23, 200000
  br i1 %exitcond41.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !295

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp ugt float %0, 0.000000e+00
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx14, align 4, !tbaa !4
  br i1 %cmp6, label %for.inc, label %L20

L20:                                              ; preds = %for.body5
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx10, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %for.body5, %L20
  %.sink = phi float [ %2, %L20 ], [ %1, %for.body5 ]
  %mul = fmul float %1, %.sink
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx12, align 4, !tbaa !4
  %add = fadd float %3, %mul
  store float %add, float* %arrayidx12, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !296
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s451(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s451, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.027 = phi i32 [ 0, %entry ], [ %inc14, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call16 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call17 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s451, i64 0, i64 0)) #11
  ret float %call17

for.cond.cleanup4:                                ; preds = %for.body5
  %call12 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc14 = add nuw nsw i32 %nl.027, 1
  %exitcond28.not = icmp eq i32 %inc14, 20000
  br i1 %exitcond28.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !297

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %call6 = tail call float @sinf(float %0) #11
  %arrayidx8 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx8, align 4, !tbaa !4
  %call9 = tail call float @cosf(float %1) #11
  %add = fadd float %call6, %call9
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx11, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !298
}

; Function Attrs: nofree nounwind optsize
declare dso_local float @sinf(float) local_unnamed_addr #2

; Function Attrs: nofree nounwind optsize
declare dso_local float @cosf(float) local_unnamed_addr #2

; Function Attrs: nounwind optsize uwtable
define dso_local float @s452(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s452, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.027 = phi i32 [ 0, %entry ], [ %inc13, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call15 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call16 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s452, i64 0, i64 0)) #11
  ret float %call16

for.cond.cleanup4:                                ; preds = %for.body5
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc13 = add nuw nsw i32 %nl.027, 1
  %exitcond28.not = icmp eq i32 %inc13, 400000
  br i1 %exitcond28.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !299

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %2 = trunc i64 %indvars.iv.next to i32
  %conv = sitofp i32 %2 to float
  %mul = fmul float %1, %conv
  %add8 = fadd float %0, %mul
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add8, float* %arrayidx10, align 4, !tbaa !4
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !300
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s453(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s453, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.025 = phi i32 [ 0, %entry ], [ %inc10, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call12 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call13 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s453, i64 0, i64 0)) #11
  ret float %call13

for.cond.cleanup4:                                ; preds = %for.body5
  %call8 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc10 = add nuw nsw i32 %nl.025, 1
  %exitcond26.not = icmp eq i32 %inc10, 200000
  br i1 %exitcond26.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !301

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %s.023 = phi float [ 0.000000e+00, %for.cond2.preheader ], [ %add, %for.body5 ]
  %add = fadd float %s.023, 2.000000e+00
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %mul = fmul float %add, %0
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %mul, float* %arrayidx7, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !302
}

; Function Attrs: norecurse nounwind optsize readnone uwtable
define dso_local i32 @s471s() local_unnamed_addr #7 {
entry:
  ret i32 0
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s471(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s471, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.044 = phi i32 [ 0, %entry ], [ %inc25, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call27 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call28 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s471, i64 0, i64 0)) #11
  ret float %call28

for.cond.cleanup4:                                ; preds = %for.body5
  %call23 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc25 = add nuw nsw i32 %nl.044, 1
  %exitcond45.not = icmp eq i32 %inc25, 50000
  br i1 %exitcond45.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !303

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %1, %1
  %add = fadd float %0, %mul
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @x, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx11, align 4, !tbaa !4
  %arrayidx14 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx14, align 4, !tbaa !4
  %arrayidx18 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx18, align 4, !tbaa !4
  %mul19 = fmul float %1, %3
  %add20 = fadd float %2, %mul19
  store float %add20, float* %arrayidx, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !304
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s481(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s481, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.029 = phi i32 [ 0, %entry ], [ %inc15, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call17 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call18 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s481, i64 0, i64 0)) #11
  ret float %call18

for.cond.cleanup4:                                ; preds = %if.end
  %call13 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc15 = add nuw nsw i32 %nl.029, 1
  %exitcond30.not = icmp eq i32 %inc15, 100000
  br i1 %exitcond30.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !305

for.body5:                                        ; preds = %for.cond2.preheader, %if.end
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %if.end ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp olt float %0, 0.000000e+00
  br i1 %cmp6, label %if.then, label %if.end

if.then:                                          ; preds = %for.body5
  tail call void @exit(i32 0) #12
  unreachable

if.end:                                           ; preds = %for.body5
  %arrayidx8 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx8, align 4, !tbaa !4
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx10, align 4, !tbaa !4
  %mul = fmul float %1, %2
  %arrayidx12 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx12, align 4, !tbaa !4
  %add = fadd float %3, %mul
  store float %add, float* %arrayidx12, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !306
}

; Function Attrs: noreturn nounwind optsize
declare dso_local void @exit(i32) local_unnamed_addr #8

; Function Attrs: nounwind optsize uwtable
define dso_local float @s482(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s482, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %cleanup
  %nl.033 = phi i32 [ 0, %entry ], [ %inc17, %cleanup ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %cleanup
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call20 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call21 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s482, i64 0, i64 0)) #11
  ret float %call21

for.cond2:                                        ; preds = %for.body5
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %cleanup, label %for.body5, !llvm.loop !307

for.body5:                                        ; preds = %for.cond2.preheader, %for.cond2
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.cond2 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %add = fadd float %2, %mul
  store float %add, float* %arrayidx9, align 4, !tbaa !4
  %cmp14 = fcmp ogt float %1, %0
  br i1 %cmp14, label %cleanup, label %for.cond2

cleanup:                                          ; preds = %for.body5, %for.cond2
  %call15 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc17 = add nuw nsw i32 %nl.033, 1
  %exitcond34.not = icmp eq i32 %inc17, 100000
  br i1 %exitcond34.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !308
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s491(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to i32**
  %1 = load i32*, i32** %0, align 8, !tbaa !43
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s491, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.032 = phi i32 [ 0, %entry ], [ %inc16, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call18 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call19 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.s491, i64 0, i64 0)) #11
  ret float %call19

for.cond.cleanup4:                                ; preds = %for.body5
  %call14 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc16 = add nuw nsw i32 %nl.032, 1
  %exitcond33.not = icmp eq i32 %inc16, 100000
  br i1 %exitcond33.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !309

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul = fmul float %3, %4
  %add = fadd float %2, %mul
  %arrayidx11 = getelementptr inbounds i32, i32* %1, i64 %indvars.iv
  %5 = load i32, i32* %arrayidx11, align 4, !tbaa !83
  %idxprom12 = sext i32 %5 to i64
  %arrayidx13 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %idxprom12
  store float %add, float* %arrayidx13, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !310
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s4112(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to %struct.anon.2**
  %1 = load %struct.anon.2*, %struct.anon.2** %0, align 8, !tbaa !43
  %a = getelementptr inbounds %struct.anon.2, %struct.anon.2* %1, i64 0, i32 0
  %2 = load i32*, i32** %a, align 8, !tbaa !311
  %b = getelementptr inbounds %struct.anon.2, %struct.anon.2* %1, i64 0, i32 1
  %3 = load float, float* %b, align 8, !tbaa !313
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s4112, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.029 = phi i32 [ 0, %entry ], [ %inc12, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call15 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s4112, i64 0, i64 0)) #11
  ret float %call15

for.cond.cleanup4:                                ; preds = %for.body5
  %call10 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc12 = add nuw nsw i32 %nl.029, 1
  %exitcond30.not = icmp eq i32 %inc12, 100000
  br i1 %exitcond30.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !314

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds i32, i32* %2, i64 %indvars.iv
  %4 = load i32, i32* %arrayidx, align 4, !tbaa !83
  %idxprom6 = sext i32 %4 to i64
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %idxprom6
  %5 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %3, %5
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %6 = load float, float* %arrayidx9, align 4, !tbaa !4
  %add = fadd float %6, %mul
  store float %add, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !315
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s4113(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to i32**
  %1 = load i32*, i32** %0, align 8, !tbaa !43
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s4113, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.032 = phi i32 [ 0, %entry ], [ %inc16, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call18 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call19 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s4113, i64 0, i64 0)) #11
  ret float %call19

for.cond.cleanup4:                                ; preds = %for.body5
  %call14 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc16 = add nuw nsw i32 %nl.032, 1
  %exitcond33.not = icmp eq i32 %inc16, 100000
  br i1 %exitcond33.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !316

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds i32, i32* %1, i64 %indvars.iv
  %2 = load i32, i32* %arrayidx, align 4, !tbaa !83
  %idxprom6 = sext i32 %2 to i64
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %idxprom6
  %3 = load float, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx9, align 4, !tbaa !4
  %add = fadd float %3, %4
  %arrayidx13 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %idxprom6
  store float %add, float* %arrayidx13, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !317
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s4114(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to %struct.anon.3**
  %1 = load %struct.anon.3*, %struct.anon.3** %0, align 8, !tbaa !43
  %a = getelementptr inbounds %struct.anon.3, %struct.anon.3* %1, i64 0, i32 0
  %2 = load i32*, i32** %a, align 8, !tbaa !318
  %b = getelementptr inbounds %struct.anon.3, %struct.anon.3* %1, i64 0, i32 1
  %3 = load i32, i32* %b, align 8, !tbaa !320
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s4114, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  %cmp340 = icmp slt i32 %3, 32001
  %4 = add i32 %3, -1
  %5 = sext i32 %4 to i64
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call22 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call23 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s4114, i64 0, i64 0)) #11
  ret float %call23

for.body:                                         ; preds = %entry, %for.cond.cleanup4
  %nl.042 = phi i32 [ 0, %entry ], [ %inc20, %for.cond.cleanup4 ]
  br i1 %cmp340, label %for.body5, label %for.cond.cleanup4

for.cond.cleanup4:                                ; preds = %for.body5, %for.body
  %call18 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc20 = add nuw nsw i32 %nl.042, 1
  %exitcond.not = icmp eq i32 %inc20, 100000
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.body, !llvm.loop !321

for.body5:                                        ; preds = %for.body, %for.body5
  %indvars.iv = phi i64 [ %indvars.iv.next, %for.body5 ], [ %5, %for.body ]
  %arrayidx = getelementptr inbounds i32, i32* %2, i64 %indvars.iv
  %6 = load i32, i32* %arrayidx, align 4, !tbaa !83
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %7 = load float, float* %arrayidx7, align 4, !tbaa !4
  %sub9 = sub i32 31999, %6
  %idxprom10 = sext i32 %sub9 to i64
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %idxprom10
  %8 = load float, float* %arrayidx11, align 4, !tbaa !4
  %arrayidx13 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %9 = load float, float* %arrayidx13, align 4, !tbaa !4
  %mul = fmul float %8, %9
  %add14 = fadd float %7, %mul
  %arrayidx16 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add14, float* %arrayidx16, align 4, !tbaa !4
  %indvars.iv.next = add nsw i64 %indvars.iv, 1
  %cmp3 = icmp slt i64 %indvars.iv, 31999
  br i1 %cmp3, label %for.body5, label %for.cond.cleanup4, !llvm.loop !322
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s4115(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to i32**
  %1 = load i32*, i32** %0, align 8, !tbaa !43
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s4115, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.028 = phi i32 [ 0, %entry ], [ %inc12, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  ret float %add

for.cond.cleanup4:                                ; preds = %for.body5
  %call10 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc12 = add nuw nsw i32 %nl.028, 1
  %exitcond29.not = icmp eq i32 %inc12, 100000
  br i1 %exitcond29.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !323

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %sum.126 = phi float [ 0.000000e+00, %for.cond2.preheader ], [ %add, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds i32, i32* %1, i64 %indvars.iv
  %3 = load i32, i32* %arrayidx7, align 4, !tbaa !83
  %idxprom8 = sext i32 %3 to i64
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %idxprom8
  %4 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul = fmul float %2, %4
  %add = fadd float %sum.126, %mul
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !324
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s4116(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to %struct.anon.4**
  %1 = load %struct.anon.4*, %struct.anon.4** %0, align 8, !tbaa !43
  %a = getelementptr inbounds %struct.anon.4, %struct.anon.4* %1, i64 0, i32 0
  %2 = load i32*, i32** %a, align 8, !tbaa !325
  %b = getelementptr inbounds %struct.anon.4, %struct.anon.4* %1, i64 0, i32 1
  %3 = load i32, i32* %b, align 8, !tbaa !327
  %c = getelementptr inbounds %struct.anon.4, %struct.anon.4* %1, i64 0, i32 2
  %4 = load i32, i32* %c, align 4, !tbaa !328
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s4116, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  %sub = add nsw i32 %3, -1
  %idxprom6 = sext i32 %sub to i64
  %5 = sext i32 %4 to i64
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.038 = phi i32 [ 0, %entry ], [ %inc16, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call18 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  ret float %add12

for.cond.cleanup4:                                ; preds = %for.body5
  %call14 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc16 = add nuw nsw i32 %nl.038, 1
  %exitcond40.not = icmp eq i32 %inc16, 10000000
  br i1 %exitcond40.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !329

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %sum.136 = phi float [ 0.000000e+00, %for.cond2.preheader ], [ %add12, %for.body5 ]
  %6 = add nsw i64 %indvars.iv, %5
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %6
  %7 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds i32, i32* %2, i64 %indvars.iv
  %8 = load i32, i32* %arrayidx9, align 4, !tbaa !83
  %idxprom10 = sext i32 %8 to i64
  %arrayidx11 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 %idxprom6, i64 %idxprom10
  %9 = load float, float* %arrayidx11, align 4, !tbaa !4
  %mul = fmul float %7, %9
  %add12 = fadd float %sum.136, %mul
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 255
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !330
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s4117(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s4117, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.028 = phi i32 [ 0, %entry ], [ %inc14, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call16 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call17 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s4117, i64 0, i64 0)) #11
  ret float %call17

for.cond.cleanup4:                                ; preds = %for.body5
  %call12 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc14 = add nuw nsw i32 %nl.028, 1
  %exitcond29.not = icmp eq i32 %inc14, 100000
  br i1 %exitcond29.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !331

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %div = lshr i64 %indvars.iv, 1
  %idxprom6 = and i64 %div, 2147483647
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %idxprom6
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul = fmul float %1, %2
  %add = fadd float %0, %mul
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %add, float* %arrayidx11, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !332
}

; Function Attrs: norecurse nounwind optsize readnone uwtable
define dso_local float @f(float %a, float %b) local_unnamed_addr #7 {
entry:
  %mul = fmul float %a, %b
  ret float %mul
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @s4121(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s4121, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.026 = phi i32 [ 0, %entry ], [ %inc13, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call15 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call16 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.s4121, i64 0, i64 0)) #11
  ret float %call16

for.cond.cleanup4:                                ; preds = %for.body5
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc13 = add nuw nsw i32 %nl.026, 1
  %exitcond27.not = icmp eq i32 %inc13, 100000
  br i1 %exitcond27.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !333

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul.i = fmul float %0, %1
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx10, align 4, !tbaa !4
  %add = fadd float %2, %mul.i
  store float %add, float* %arrayidx10, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !334
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @va(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @__func__.va, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond2.preheader
  %nl.022 = phi i32 [ 0, %entry ], [ %inc10, %for.cond2.preheader ]
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 64 dereferenceable(128000) bitcast ([32000 x float]* @a to i8*), i8* nonnull align 64 dereferenceable(128000) bitcast ([32000 x float]* @b to i8*), i64 128000, i1 false)
  %call8 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc10 = add nuw nsw i32 %nl.022, 1
  %exitcond.not = icmp eq i32 %inc10, 1000000
  br i1 %exitcond.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !335

for.cond.cleanup:                                 ; preds = %for.cond2.preheader
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call12 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call13 = tail call float @calc_checksum(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @__func__.va, i64 0, i64 0)) #11
  ret float %call13
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @vag(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to i32**
  %1 = load i32*, i32** %0, align 8, !tbaa !43
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @__func__.vag, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.026 = phi i32 [ 0, %entry ], [ %inc12, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call15 = tail call float @calc_checksum(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @__func__.vag, i64 0, i64 0)) #11
  ret float %call15

for.cond.cleanup4:                                ; preds = %for.body5
  %call10 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc12 = add nuw nsw i32 %nl.026, 1
  %exitcond27.not = icmp eq i32 %inc12, 200000
  br i1 %exitcond27.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !336

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds i32, i32* %1, i64 %indvars.iv
  %2 = load i32, i32* %arrayidx, align 4, !tbaa !83
  %idxprom6 = sext i32 %2 to i64
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %idxprom6
  %3 = load float, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %3, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !337
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @vas(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to i32**
  %1 = load i32*, i32** %0, align 8, !tbaa !43
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @__func__.vas, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.026 = phi i32 [ 0, %entry ], [ %inc12, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call15 = tail call float @calc_checksum(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @__func__.vas, i64 0, i64 0)) #11
  ret float %call15

for.cond.cleanup4:                                ; preds = %for.body5
  %call10 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc12 = add nuw nsw i32 %nl.026, 1
  %exitcond27.not = icmp eq i32 %inc12, 200000
  br i1 %exitcond27.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !338

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds i32, i32* %1, i64 %indvars.iv
  %3 = load i32, i32* %arrayidx7, align 4, !tbaa !83
  %idxprom8 = sext i32 %3 to i64
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %idxprom8
  store float %2, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !339
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @vif(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @__func__.vif, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.026 = phi i32 [ 0, %entry ], [ %inc13, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call15 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call16 = tail call float @calc_checksum(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @__func__.vif, i64 0, i64 0)) #11
  ret float %call16

for.cond.cleanup4:                                ; preds = %for.inc
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc13 = add nuw nsw i32 %nl.026, 1
  %exitcond27.not = icmp eq i32 %inc13, 100000
  br i1 %exitcond27.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !340

for.body5:                                        ; preds = %for.cond2.preheader, %for.inc
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.inc ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %cmp6 = fcmp ogt float %0, 0.000000e+00
  br i1 %cmp6, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body5
  %arrayidx10 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  store float %0, float* %arrayidx10, align 4, !tbaa !4
  br label %for.inc

for.inc:                                          ; preds = %for.body5, %if.then
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !341
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @vpv(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @__func__.vpv, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.022 = phi i32 [ 0, %entry ], [ %inc10, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call12 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call13 = tail call float @calc_checksum(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @__func__.vpv, i64 0, i64 0)) #11
  ret float %call13

for.cond.cleanup4:                                ; preds = %for.body5
  %call8 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc10 = add nuw nsw i32 %nl.022, 1
  %exitcond23.not = icmp eq i32 %inc10, 1000000
  br i1 %exitcond23.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !342

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %0, %1
  store float %add, float* %arrayidx7, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !343
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @vtv(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @__func__.vtv, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.022 = phi i32 [ 0, %entry ], [ %inc10, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call12 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call13 = tail call float @calc_checksum(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @__func__.vtv, i64 0, i64 0)) #11
  ret float %call13

for.cond.cleanup4:                                ; preds = %for.body5
  %call8 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc10 = add nuw nsw i32 %nl.022, 1
  %exitcond23.not = icmp eq i32 %inc10, 1000000
  br i1 %exitcond23.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !344

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %0, %1
  store float %mul, float* %arrayidx7, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !345
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @vpvtv(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.vpvtv, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.025 = phi i32 [ 0, %entry ], [ %inc12, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call15 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.vpvtv, i64 0, i64 0)) #11
  ret float %call15

for.cond.cleanup4:                                ; preds = %for.body5
  %call10 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc12 = add nuw nsw i32 %nl.025, 1
  %exitcond26.not = icmp eq i32 %inc12, 400000
  br i1 %exitcond26.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !346

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %add = fadd float %2, %mul
  store float %add, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !347
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @vpvts(%struct.args_t* nocapture %func_args) #0 {
entry:
  %arg_info = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  %0 = bitcast i8** %arg_info to i32**
  %1 = load i32*, i32** %0, align 8, !tbaa !43
  %2 = load i32, i32* %1, align 4, !tbaa !83
  %conv = sitofp i32 %2 to float
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.vpvts, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond3.preheader

for.cond3.preheader:                              ; preds = %entry, %for.cond.cleanup6
  %nl.026 = phi i32 [ 0, %entry ], [ %inc12, %for.cond.cleanup6 ]
  br label %for.body7

for.cond.cleanup:                                 ; preds = %for.cond.cleanup6
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call14 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call15 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.vpvts, i64 0, i64 0)) #11
  ret float %call15

for.cond.cleanup6:                                ; preds = %for.body7
  %call10 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc12 = add nuw nsw i32 %nl.026, 1
  %exitcond27.not = icmp eq i32 %inc12, 100000
  br i1 %exitcond27.not, label %for.cond.cleanup, label %for.cond3.preheader, !llvm.loop !348

for.body7:                                        ; preds = %for.cond3.preheader, %for.body7
  %indvars.iv = phi i64 [ 0, %for.cond3.preheader ], [ %indvars.iv.next, %for.body7 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx, align 4, !tbaa !4
  %mul = fmul float %3, %conv
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx9, align 4, !tbaa !4
  %add = fadd float %4, %mul
  store float %add, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup6, label %for.body7, !llvm.loop !349
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @vpvpv(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.vpvpv, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.026 = phi i32 [ 0, %entry ], [ %inc13, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call15 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call16 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.vpvpv, i64 0, i64 0)) #11
  ret float %call16

for.cond.cleanup4:                                ; preds = %for.body5
  %call11 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc13 = add nuw nsw i32 %nl.026, 1
  %exitcond27.not = icmp eq i32 %inc13, 400000
  br i1 %exitcond27.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !350

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %add = fadd float %0, %1
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %add10 = fadd float %2, %add
  store float %add10, float* %arrayidx9, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !351
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @vtvtv(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.vtvtv, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.029 = phi i32 [ 0, %entry ], [ %inc15, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call17 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call18 = tail call float @calc_checksum(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.vtvtv, i64 0, i64 0)) #11
  ret float %call18

for.cond.cleanup4:                                ; preds = %for.body5
  %call13 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc15 = add nuw nsw i32 %nl.029, 1
  %exitcond30.not = icmp eq i32 %inc15, 400000
  br i1 %exitcond30.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !352

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %mul10 = fmul float %mul, %2
  store float %mul10, float* %arrayidx, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !353
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @vsumr(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.vsumr, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.022 = phi i32 [ 0, %entry ], [ %inc8, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call10 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  ret float %add

for.cond.cleanup4:                                ; preds = %for.body5
  %call6 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %add) #11
  %inc8 = add nuw nsw i32 %nl.022, 1
  %exitcond23.not = icmp eq i32 %inc8, 1000000
  br i1 %exitcond23.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !354

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %sum.120 = phi float [ 0.000000e+00, %for.cond2.preheader ], [ %add, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %add = fadd float %sum.120, %0
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !355
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @vdotr(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @__func__.vdotr, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.025 = phi i32 [ 0, %entry ], [ %inc10, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call12 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  ret float %add

for.cond.cleanup4:                                ; preds = %for.body5
  %call8 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float %add) #11
  %inc10 = add nuw nsw i32 %nl.025, 1
  %exitcond26.not = icmp eq i32 %inc10, 1000000
  br i1 %exitcond26.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !356

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %dot.123 = phi float [ 0.000000e+00, %for.cond2.preheader ], [ %add, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %add = fadd float %dot.123, %mul
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 32000
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !357
}

; Function Attrs: nounwind optsize uwtable
define dso_local float @vbor(%struct.args_t* nocapture %func_args) #0 {
entry:
  %call = tail call i32 @initialise_arrays(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.vbor, i64 0, i64 0)) #11
  %t1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0
  %call1 = tail call i32 @gettimeofday(%struct.timeval* %t1, i8* null) #11
  br label %for.cond2.preheader

for.cond2.preheader:                              ; preds = %entry, %for.cond.cleanup4
  %nl.0158 = phi i32 [ 0, %entry ], [ %inc77, %for.cond.cleanup4 ]
  br label %for.body5

for.cond.cleanup:                                 ; preds = %for.cond.cleanup4
  %t2 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1
  %call79 = tail call i32 @gettimeofday(%struct.timeval* nonnull %t2, i8* null) #11
  %call80 = tail call float @calc_checksum(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.vbor, i64 0, i64 0)) #11
  ret float %call80

for.cond.cleanup4:                                ; preds = %for.body5
  %call75 = tail call i32 @dummy(float* getelementptr inbounds ([32000 x float], [32000 x float]* @a, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @b, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @c, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @d, i64 0, i64 0), float* getelementptr inbounds ([32000 x float], [32000 x float]* @e, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @bb, i64 0, i64 0), [256 x float]* getelementptr inbounds ([256 x [256 x float]], [256 x [256 x float]]* @cc, i64 0, i64 0), float 0.000000e+00) #11
  %inc77 = add nuw nsw i32 %nl.0158, 1
  %exitcond159.not = icmp eq i32 %inc77, 1000000
  br i1 %exitcond159.not, label %for.cond.cleanup, label %for.cond2.preheader, !llvm.loop !358

for.body5:                                        ; preds = %for.cond2.preheader, %for.body5
  %indvars.iv = phi i64 [ 0, %for.cond2.preheader ], [ %indvars.iv.next, %for.body5 ]
  %arrayidx = getelementptr inbounds [32000 x float], [32000 x float]* @a, i64 0, i64 %indvars.iv
  %0 = load float, float* %arrayidx, align 4, !tbaa !4
  %arrayidx7 = getelementptr inbounds [32000 x float], [32000 x float]* @b, i64 0, i64 %indvars.iv
  %1 = load float, float* %arrayidx7, align 4, !tbaa !4
  %arrayidx9 = getelementptr inbounds [32000 x float], [32000 x float]* @c, i64 0, i64 %indvars.iv
  %2 = load float, float* %arrayidx9, align 4, !tbaa !4
  %arrayidx11 = getelementptr inbounds [32000 x float], [32000 x float]* @d, i64 0, i64 %indvars.iv
  %3 = load float, float* %arrayidx11, align 4, !tbaa !4
  %arrayidx13 = getelementptr inbounds [32000 x float], [32000 x float]* @e, i64 0, i64 %indvars.iv
  %4 = load float, float* %arrayidx13, align 4, !tbaa !4
  %arrayidx15 = getelementptr inbounds [256 x [256 x float]], [256 x [256 x float]]* @aa, i64 0, i64 0, i64 %indvars.iv
  %5 = load float, float* %arrayidx15, align 4, !tbaa !4
  %mul = fmul float %0, %1
  %mul16 = fmul float %mul, %2
  %mul18 = fmul float %mul, %3
  %add = fadd float %mul16, %mul18
  %mul20 = fmul float %mul, %4
  %add21 = fadd float %add, %mul20
  %mul23 = fmul float %mul, %5
  %add24 = fadd float %add21, %mul23
  %mul25 = fmul float %0, %2
  %mul26 = fmul float %mul25, %3
  %add27 = fadd float %mul26, %add24
  %mul29 = fmul float %mul25, %4
  %add30 = fadd float %mul29, %add27
  %mul32 = fmul float %mul25, %5
  %add33 = fadd float %mul32, %add30
  %mul34 = fmul float %0, %3
  %mul35 = fmul float %mul34, %4
  %add36 = fadd float %mul35, %add33
  %mul38 = fmul float %mul34, %5
  %add39 = fadd float %mul38, %add36
  %mul40 = fmul float %0, %4
  %mul41 = fmul float %mul40, %5
  %add42 = fadd float %mul41, %add39
  %mul43 = fmul float %1, %2
  %mul44 = fmul float %mul43, %3
  %mul46 = fmul float %mul43, %4
  %add47 = fadd float %mul44, %mul46
  %mul49 = fmul float %mul43, %5
  %add50 = fadd float %add47, %mul49
  %mul51 = fmul float %1, %3
  %mul52 = fmul float %mul51, %4
  %add53 = fadd float %mul52, %add50
  %mul55 = fmul float %mul51, %5
  %add56 = fadd float %mul55, %add53
  %mul57 = fmul float %1, %4
  %mul58 = fmul float %mul57, %5
  %add59 = fadd float %mul58, %add56
  %mul60 = fmul float %2, %3
  %mul61 = fmul float %mul60, %4
  %mul63 = fmul float %mul60, %5
  %add64 = fadd float %mul61, %mul63
  %mul65 = fmul float %2, %4
  %mul66 = fmul float %mul65, %5
  %add67 = fadd float %mul66, %add64
  %mul68 = fmul float %3, %4
  %mul69 = fmul float %mul68, %5
  %mul70 = fmul float %add59, %add42
  %mul71 = fmul float %add67, %mul70
  %mul72 = fmul float %mul69, %mul71
  %arrayidx74 = getelementptr inbounds [32000 x float], [32000 x float]* @x, i64 0, i64 %indvars.iv
  store float %mul72, float* %arrayidx74, align 4, !tbaa !4
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 256
  br i1 %exitcond.not, label %for.cond.cleanup4, label %for.body5, !llvm.loop !359
}

; Function Attrs: nounwind optsize uwtable
define dso_local void @time_function(float (%struct.args_t*)* nocapture %vector_func, i8* %arg_info) local_unnamed_addr #0 {
entry:
  %func_args = alloca %struct.args_t, align 8
  %0 = bitcast %struct.args_t* %func_args to i8*
  call void @llvm.lifetime.start.p0i8(i64 40, i8* nonnull %0) #13
  call void @llvm.memset.p0i8.i64(i8* nonnull align 8 dereferenceable(40) %0, i8 0, i64 32, i1 false)
  %arg_info1 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 2
  store i8* %arg_info, i8** %arg_info1, align 8, !tbaa !43
  %call = call float %vector_func(%struct.args_t* nonnull %func_args) #11
  %conv = fpext float %call to double
  %tv_sec = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0, i32 0
  %1 = load i64, i64* %tv_sec, align 8, !tbaa !360
  %conv2 = sitofp i64 %1 to double
  %tv_usec = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 0, i32 1
  %2 = load i64, i64* %tv_usec, align 8, !tbaa !361
  %conv4 = sitofp i64 %2 to double
  %div = fdiv double %conv4, 1.000000e+06
  %add = fadd double %div, %conv2
  %tv_sec5 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1, i32 0
  %3 = load i64, i64* %tv_sec5, align 8, !tbaa !362
  %conv6 = sitofp i64 %3 to double
  %tv_usec8 = getelementptr inbounds %struct.args_t, %struct.args_t* %func_args, i64 0, i32 1, i32 1
  %4 = load i64, i64* %tv_usec8, align 8, !tbaa !363
  %conv9 = sitofp i64 %4 to double
  %div10 = fdiv double %conv9, 1.000000e+06
  %add11 = fadd double %div10, %conv6
  %sub = fsub double %add11, %add
  %call12 = call i32 (i8*, ...) @printf(i8* nonnull dereferenceable(1) getelementptr inbounds ([11 x i8], [11 x i8]* @.str, i64 0, i64 0), double %sub, double %conv) #14
  call void @llvm.lifetime.end.p0i8(i64 40, i8* nonnull %0) #13
  ret void
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #9

; Function Attrs: nofree nounwind optsize
declare dso_local noundef i32 @printf(i8* nocapture noundef readonly, ...) local_unnamed_addr #2

; Function Attrs: nounwind optsize uwtable
define dso_local i32 @main(i32 %argc, i8** nocapture readnone %argv) local_unnamed_addr #0 {
entry:
  %n1 = alloca i32, align 4
  %ip = alloca i32*, align 8
  %s1 = alloca float, align 4
  %s2 = alloca float, align 4
  %.compoundliteral = alloca %struct.anon.5, align 4
  %.compoundliteral1 = alloca %struct.anon.6, align 4
  %.compoundliteral4 = alloca %struct.anon.7, align 4
  %.compoundliteral6 = alloca %struct.anon.8, align 4
  %.compoundliteral9 = alloca %struct.anon.9, align 8
  %.compoundliteral12 = alloca %struct.anon.10, align 8
  %.compoundliteral15 = alloca %struct.anon.11, align 8
  %0 = bitcast i32* %n1 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %0) #13
  store i32 1, i32* %n1, align 4, !tbaa !83
  %1 = bitcast i32** %ip to i8*
  call void @llvm.lifetime.start.p0i8(i64 8, i8* nonnull %1) #13
  %2 = bitcast float* %s1 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %2) #13
  %3 = bitcast float* %s2 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %3) #13
  call void @init(i32** nonnull %ip, float* nonnull %s1, float* nonnull %s2) #11
  %puts = call i32 @puts(i8* nonnull dereferenceable(1) getelementptr inbounds ([26 x i8], [26 x i8]* @str, i64 0, i64 0))
  call void @time_function(float (%struct.args_t*)* nonnull @s000, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s111, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s1111, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s112, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s1112, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s113, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s1113, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s114, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s115, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s1115, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s116, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s118, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s119, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s1119, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s121, i8* null) #14
  %a = getelementptr inbounds %struct.anon.5, %struct.anon.5* %.compoundliteral, i64 0, i32 0
  store i32 1, i32* %a, align 4, !tbaa !48
  %b = getelementptr inbounds %struct.anon.5, %struct.anon.5* %.compoundliteral, i64 0, i32 1
  store i32 1, i32* %b, align 4, !tbaa !51
  %4 = bitcast %struct.anon.5* %.compoundliteral to i8*
  call void @time_function(float (%struct.args_t*)* nonnull @s122, i8* nonnull %4) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s123, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s124, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s125, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s126, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s127, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s128, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s131, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s132, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s141, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s151, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s152, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s161, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s1161, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s162, i8* nonnull %0) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s171, i8* nonnull %0) #14
  %a2 = getelementptr inbounds %struct.anon.6, %struct.anon.6* %.compoundliteral1, i64 0, i32 0
  %5 = load i32, i32* %n1, align 4, !tbaa !83
  store i32 %5, i32* %a2, align 4, !tbaa !48
  %b3 = getelementptr inbounds %struct.anon.6, %struct.anon.6* %.compoundliteral1, i64 0, i32 1
  store i32 1, i32* %b3, align 4, !tbaa !51
  %6 = bitcast %struct.anon.6* %.compoundliteral1 to i8*
  call void @time_function(float (%struct.args_t*)* nonnull @s172, i8* nonnull %6) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s173, i8* null) #14
  %a5 = getelementptr inbounds %struct.anon.7, %struct.anon.7* %.compoundliteral4, i64 0, i32 0
  store i32 16000, i32* %a5, align 4, !tbaa !364
  %7 = bitcast %struct.anon.7* %.compoundliteral4 to i8*
  call void @time_function(float (%struct.args_t*)* nonnull @s174, i8* nonnull %7) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s175, i8* nonnull %0) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s176, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s211, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s212, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s1213, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s221, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s1221, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s222, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s231, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s232, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s1232, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s233, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s2233, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s235, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s241, i8* null) #14
  %a7 = getelementptr inbounds %struct.anon.8, %struct.anon.8* %.compoundliteral6, i64 0, i32 0
  %8 = load float, float* %s1, align 4, !tbaa !4
  store float %8, float* %a7, align 4, !tbaa !133
  %b8 = getelementptr inbounds %struct.anon.8, %struct.anon.8* %.compoundliteral6, i64 0, i32 1
  %9 = load float, float* %s2, align 4, !tbaa !4
  store float %9, float* %b8, align 4, !tbaa !135
  %10 = bitcast %struct.anon.8* %.compoundliteral6 to i8*
  call void @time_function(float (%struct.args_t*)* nonnull @s242, i8* nonnull %10) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s243, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s244, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s1244, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s2244, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s251, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s1251, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s2251, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s3251, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s252, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s253, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s254, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s255, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s256, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s257, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s258, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s261, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s271, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s272, i8* nonnull %2) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s273, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s274, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s275, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s2275, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s276, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s277, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s278, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s279, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s1279, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s2710, i8* nonnull %2) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s2711, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s2712, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s281, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s1281, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s291, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s292, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s293, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s2101, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s2102, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s2111, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s311, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s31111, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s312, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s313, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s314, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s315, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s316, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s317, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s318, i8* nonnull %0) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s319, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s3110, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s13110, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s3111, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s3112, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s3113, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s321, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s322, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s323, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s331, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s332, i8* nonnull %2) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s341, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s342, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s343, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s351, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s1351, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s352, i8* null) #14
  %11 = bitcast i32** %ip to i8**
  %12 = load i8*, i8** %11, align 8, !tbaa !278
  call void @time_function(float (%struct.args_t*)* nonnull @s353, i8* %12) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s421, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s1421, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s422, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s423, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s424, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s431, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s441, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s442, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s443, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s451, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s452, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s453, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s471, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s481, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s482, i8* null) #14
  %13 = load i8*, i8** %11, align 8, !tbaa !278
  call void @time_function(float (%struct.args_t*)* nonnull @s491, i8* %13) #14
  %a10 = getelementptr inbounds %struct.anon.9, %struct.anon.9* %.compoundliteral9, i64 0, i32 0
  %14 = load i32*, i32** %ip, align 8, !tbaa !278
  store i32* %14, i32** %a10, align 8, !tbaa !311
  %b11 = getelementptr inbounds %struct.anon.9, %struct.anon.9* %.compoundliteral9, i64 0, i32 1
  %15 = load float, float* %s1, align 4, !tbaa !4
  store float %15, float* %b11, align 8, !tbaa !313
  %16 = bitcast %struct.anon.9* %.compoundliteral9 to i8*
  call void @time_function(float (%struct.args_t*)* nonnull @s4112, i8* nonnull %16) #14
  %17 = load i8*, i8** %11, align 8, !tbaa !278
  call void @time_function(float (%struct.args_t*)* nonnull @s4113, i8* %17) #14
  %a13 = getelementptr inbounds %struct.anon.10, %struct.anon.10* %.compoundliteral12, i64 0, i32 0
  %18 = load i32*, i32** %ip, align 8, !tbaa !278
  store i32* %18, i32** %a13, align 8, !tbaa !318
  %b14 = getelementptr inbounds %struct.anon.10, %struct.anon.10* %.compoundliteral12, i64 0, i32 1
  %19 = load i32, i32* %n1, align 4, !tbaa !83
  store i32 %19, i32* %b14, align 8, !tbaa !320
  %20 = bitcast %struct.anon.10* %.compoundliteral12 to i8*
  call void @time_function(float (%struct.args_t*)* nonnull @s4114, i8* nonnull %20) #14
  %21 = load i8*, i8** %11, align 8, !tbaa !278
  call void @time_function(float (%struct.args_t*)* nonnull @s4115, i8* %21) #14
  %a16 = getelementptr inbounds %struct.anon.11, %struct.anon.11* %.compoundliteral15, i64 0, i32 0
  %22 = load i32*, i32** %ip, align 8, !tbaa !278
  store i32* %22, i32** %a16, align 8, !tbaa !325
  %b17 = getelementptr inbounds %struct.anon.11, %struct.anon.11* %.compoundliteral15, i64 0, i32 1
  store i32 128, i32* %b17, align 8, !tbaa !327
  %c = getelementptr inbounds %struct.anon.11, %struct.anon.11* %.compoundliteral15, i64 0, i32 2
  %23 = load i32, i32* %n1, align 4, !tbaa !83
  store i32 %23, i32* %c, align 4, !tbaa !328
  %24 = bitcast %struct.anon.11* %.compoundliteral15 to i8*
  call void @time_function(float (%struct.args_t*)* nonnull @s4116, i8* nonnull %24) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s4117, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @s4121, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @va, i8* null) #14
  %25 = load i8*, i8** %11, align 8, !tbaa !278
  call void @time_function(float (%struct.args_t*)* nonnull @vag, i8* %25) #14
  %26 = load i8*, i8** %11, align 8, !tbaa !278
  call void @time_function(float (%struct.args_t*)* nonnull @vas, i8* %26) #14
  call void @time_function(float (%struct.args_t*)* nonnull @vif, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @vpv, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @vtv, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @vpvtv, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @vpvts, i8* nonnull %2) #14
  call void @time_function(float (%struct.args_t*)* nonnull @vpvpv, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @vtvtv, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @vsumr, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @vdotr, i8* null) #14
  call void @time_function(float (%struct.args_t*)* nonnull @vbor, i8* null) #14
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %3) #13
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %2) #13
  call void @llvm.lifetime.end.p0i8(i64 8, i8* nonnull %1) #13
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %0) #13
  ret i32 0
}

; Function Attrs: optsize
declare dso_local void @init(i32**, float*, float*) local_unnamed_addr #1

; Function Attrs: nofree nounwind
declare noundef i32 @puts(i8* nocapture noundef readonly) local_unnamed_addr #10

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #3

attributes #0 = { nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { optsize "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nofree nounwind optsize "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { argmemonly nofree nosync nounwind willreturn }
attributes #4 = { nofree norecurse nounwind optsize uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { norecurse nounwind optsize readonly uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #7 = { norecurse nounwind optsize readnone uwtable "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { noreturn nounwind optsize "disable-tail-calls"="false" "frame-pointer"="none" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #9 = { argmemonly nofree nosync nounwind willreturn writeonly }
attributes #10 = { nofree nounwind }
attributes #11 = { nounwind optsize }
attributes #12 = { noreturn nounwind optsize }
attributes #13 = { nounwind }
attributes #14 = { optsize }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 12.0.0 (https://github.com/rcorcs/llvm-project.git 119a5ecb40119a87b443629d5136121ab0df5bcb)"}
!2 = distinct !{!2, !3}
!3 = !{!"llvm.loop.unroll.disable"}
!4 = !{!5, !5, i64 0}
!5 = !{!"float", !6, i64 0}
!6 = !{!"omnipotent char", !7, i64 0}
!7 = !{!"Simple C/C++ TBAA"}
!8 = distinct !{!8, !3}
!9 = distinct !{!9, !3}
!10 = distinct !{!10, !3}
!11 = distinct !{!11, !3}
!12 = distinct !{!12, !3}
!13 = distinct !{!13, !3}
!14 = distinct !{!14, !3}
!15 = distinct !{!15, !3}
!16 = distinct !{!16, !3}
!17 = distinct !{!17, !3}
!18 = distinct !{!18, !3}
!19 = distinct !{!19, !3}
!20 = distinct !{!20, !3}
!21 = distinct !{!21, !3}
!22 = distinct !{!22, !3}
!23 = distinct !{!23, !3}
!24 = distinct !{!24, !3}
!25 = distinct !{!25, !3}
!26 = distinct !{!26, !3}
!27 = distinct !{!27, !3}
!28 = distinct !{!28, !3}
!29 = distinct !{!29, !3}
!30 = distinct !{!30, !3}
!31 = distinct !{!31, !3}
!32 = distinct !{!32, !3}
!33 = distinct !{!33, !3}
!34 = distinct !{!34, !3}
!35 = distinct !{!35, !3}
!36 = distinct !{!36, !3}
!37 = distinct !{!37, !3}
!38 = distinct !{!38, !3}
!39 = distinct !{!39, !3}
!40 = distinct !{!40, !3}
!41 = distinct !{!41, !3}
!42 = distinct !{!42, !3}
!43 = !{!44, !47, i64 32}
!44 = !{!"args_t", !45, i64 0, !45, i64 16, !47, i64 32}
!45 = !{!"timeval", !46, i64 0, !46, i64 8}
!46 = !{!"long", !6, i64 0}
!47 = !{!"any pointer", !6, i64 0}
!48 = !{!49, !50, i64 0}
!49 = !{!"", !50, i64 0, !50, i64 4}
!50 = !{!"int", !6, i64 0}
!51 = !{!49, !50, i64 4}
!52 = distinct !{!52, !3}
!53 = distinct !{!53, !3}
!54 = distinct !{!54, !3}
!55 = distinct !{!55, !3}
!56 = distinct !{!56, !3}
!57 = distinct !{!57, !3}
!58 = distinct !{!58, !3}
!59 = distinct !{!59, !3}
!60 = distinct !{!60, !3}
!61 = distinct !{!61, !3}
!62 = distinct !{!62, !3}
!63 = distinct !{!63, !3}
!64 = distinct !{!64, !3}
!65 = distinct !{!65, !3}
!66 = distinct !{!66, !3}
!67 = distinct !{!67, !3}
!68 = distinct !{!68, !3}
!69 = distinct !{!69, !3}
!70 = distinct !{!70, !3}
!71 = distinct !{!71, !3}
!72 = distinct !{!72, !3}
!73 = distinct !{!73, !3}
!74 = distinct !{!74, !3}
!75 = distinct !{!75, !3}
!76 = distinct !{!76, !3}
!77 = distinct !{!77, !3}
!78 = distinct !{!78, !3}
!79 = distinct !{!79, !3}
!80 = distinct !{!80, !3}
!81 = distinct !{!81, !3}
!82 = distinct !{!82, !3}
!83 = !{!50, !50, i64 0}
!84 = distinct !{!84, !3}
!85 = distinct !{!85, !3}
!86 = distinct !{!86, !3}
!87 = distinct !{!87, !3}
!88 = distinct !{!88, !3}
!89 = distinct !{!89, !3}
!90 = distinct !{!90, !3}
!91 = distinct !{!91, !3}
!92 = distinct !{!92, !3}
!93 = distinct !{!93, !3}
!94 = distinct !{!94, !3}
!95 = distinct !{!95, !3}
!96 = distinct !{!96, !3}
!97 = distinct !{!97, !3}
!98 = distinct !{!98, !3}
!99 = distinct !{!99, !3}
!100 = distinct !{!100, !3}
!101 = distinct !{!101, !3}
!102 = distinct !{!102, !3}
!103 = distinct !{!103, !3}
!104 = distinct !{!104, !3}
!105 = distinct !{!105, !3}
!106 = distinct !{!106, !3}
!107 = distinct !{!107, !3}
!108 = distinct !{!108, !3}
!109 = distinct !{!109, !3}
!110 = distinct !{!110, !3}
!111 = distinct !{!111, !3}
!112 = distinct !{!112, !3}
!113 = distinct !{!113, !3}
!114 = distinct !{!114, !3}
!115 = distinct !{!115, !3}
!116 = distinct !{!116, !3}
!117 = distinct !{!117, !3}
!118 = distinct !{!118, !3}
!119 = distinct !{!119, !3}
!120 = distinct !{!120, !3}
!121 = distinct !{!121, !3}
!122 = distinct !{!122, !3}
!123 = distinct !{!123, !3}
!124 = distinct !{!124, !3}
!125 = distinct !{!125, !3}
!126 = distinct !{!126, !3}
!127 = distinct !{!127, !3}
!128 = distinct !{!128, !3}
!129 = distinct !{!129, !3}
!130 = distinct !{!130, !3}
!131 = distinct !{!131, !3}
!132 = distinct !{!132, !3}
!133 = !{!134, !5, i64 0}
!134 = !{!"", !5, i64 0, !5, i64 4}
!135 = !{!134, !5, i64 4}
!136 = distinct !{!136, !3}
!137 = distinct !{!137, !3}
!138 = distinct !{!138, !3}
!139 = distinct !{!139, !3}
!140 = distinct !{!140, !3}
!141 = distinct !{!141, !3}
!142 = distinct !{!142, !3}
!143 = distinct !{!143, !3}
!144 = distinct !{!144, !3}
!145 = distinct !{!145, !3}
!146 = distinct !{!146, !3}
!147 = distinct !{!147, !3}
!148 = distinct !{!148, !3}
!149 = distinct !{!149, !3}
!150 = distinct !{!150, !3}
!151 = distinct !{!151, !3}
!152 = distinct !{!152, !3}
!153 = distinct !{!153, !3}
!154 = distinct !{!154, !3}
!155 = distinct !{!155, !3}
!156 = distinct !{!156, !3}
!157 = distinct !{!157, !3}
!158 = distinct !{!158, !3}
!159 = distinct !{!159, !3}
!160 = distinct !{!160, !3}
!161 = distinct !{!161, !3}
!162 = distinct !{!162, !3}
!163 = distinct !{!163, !3}
!164 = distinct !{!164, !3}
!165 = distinct !{!165, !3}
!166 = distinct !{!166, !3}
!167 = distinct !{!167, !3}
!168 = distinct !{!168, !3}
!169 = distinct !{!169, !3}
!170 = distinct !{!170, !3}
!171 = distinct !{!171, !3}
!172 = distinct !{!172, !3}
!173 = distinct !{!173, !3}
!174 = distinct !{!174, !3}
!175 = distinct !{!175, !3}
!176 = distinct !{!176, !3}
!177 = distinct !{!177, !3}
!178 = distinct !{!178, !3}
!179 = distinct !{!179, !3}
!180 = distinct !{!180, !3}
!181 = distinct !{!181, !3}
!182 = distinct !{!182, !3}
!183 = distinct !{!183, !3}
!184 = distinct !{!184, !3}
!185 = distinct !{!185, !3}
!186 = distinct !{!186, !3}
!187 = distinct !{!187, !3}
!188 = distinct !{!188, !3}
!189 = distinct !{!189, !3}
!190 = distinct !{!190, !3}
!191 = distinct !{!191, !3}
!192 = distinct !{!192, !3}
!193 = distinct !{!193, !3}
!194 = distinct !{!194, !3}
!195 = distinct !{!195, !3}
!196 = distinct !{!196, !3}
!197 = distinct !{!197, !3}
!198 = distinct !{!198, !3}
!199 = distinct !{!199, !3}
!200 = distinct !{!200, !3}
!201 = distinct !{!201, !3}
!202 = distinct !{!202, !3}
!203 = distinct !{!203, !3}
!204 = distinct !{!204, !3}
!205 = distinct !{!205, !3}
!206 = distinct !{!206, !3}
!207 = distinct !{!207, !3}
!208 = distinct !{!208, !3}
!209 = distinct !{!209, !3}
!210 = distinct !{!210, !3}
!211 = distinct !{!211, !3}
!212 = distinct !{!212, !3}
!213 = distinct !{!213, !3}
!214 = distinct !{!214, !3}
!215 = distinct !{!215, !3}
!216 = distinct !{!216, !3}
!217 = distinct !{!217, !3}
!218 = distinct !{!218, !3}
!219 = distinct !{!219, !3}
!220 = distinct !{!220, !3}
!221 = distinct !{!221, !3}
!222 = distinct !{!222, !3}
!223 = distinct !{!223, !3}
!224 = distinct !{!224, !3}
!225 = distinct !{!225, !3}
!226 = distinct !{!226, !3}
!227 = distinct !{!227, !3}
!228 = distinct !{!228, !3}
!229 = distinct !{!229, !3}
!230 = distinct !{!230, !3}
!231 = distinct !{!231, !3}
!232 = distinct !{!232, !3}
!233 = distinct !{!233, !3}
!234 = distinct !{!234, !3}
!235 = distinct !{!235, !3}
!236 = distinct !{!236, !3}
!237 = distinct !{!237, !3}
!238 = distinct !{!238, !3}
!239 = distinct !{!239, !3}
!240 = distinct !{!240, !3}
!241 = distinct !{!241, !3}
!242 = distinct !{!242, !3}
!243 = distinct !{!243, !3}
!244 = distinct !{!244, !3}
!245 = distinct !{!245, !3}
!246 = distinct !{!246, !3}
!247 = distinct !{!247, !3}
!248 = distinct !{!248, !3}
!249 = distinct !{!249, !3}
!250 = distinct !{!250, !3}
!251 = distinct !{!251, !3}
!252 = distinct !{!252, !3}
!253 = distinct !{!253, !3}
!254 = distinct !{!254, !3}
!255 = distinct !{!255, !3}
!256 = distinct !{!256, !3}
!257 = distinct !{!257, !3}
!258 = distinct !{!258, !3}
!259 = distinct !{!259, !3}
!260 = distinct !{!260, !3}
!261 = distinct !{!261, !3}
!262 = distinct !{!262, !3}
!263 = distinct !{!263, !3}
!264 = distinct !{!264, !3}
!265 = distinct !{!265, !3}
!266 = distinct !{!266, !3}
!267 = distinct !{!267, !3}
!268 = distinct !{!268, !3}
!269 = distinct !{!269, !3}
!270 = distinct !{!270, !3}
!271 = distinct !{!271, !3}
!272 = distinct !{!272, !3}
!273 = distinct !{!273, !3}
!274 = distinct !{!274, !3}
!275 = distinct !{!275, !3}
!276 = distinct !{!276, !3}
!277 = distinct !{!277, !3}
!278 = !{!47, !47, i64 0}
!279 = distinct !{!279, !3}
!280 = distinct !{!280, !3}
!281 = distinct !{!281, !3}
!282 = distinct !{!282, !3}
!283 = distinct !{!283, !3}
!284 = distinct !{!284, !3}
!285 = distinct !{!285, !3}
!286 = distinct !{!286, !3}
!287 = distinct !{!287, !3}
!288 = distinct !{!288, !3}
!289 = distinct !{!289, !3}
!290 = distinct !{!290, !3}
!291 = distinct !{!291, !3}
!292 = distinct !{!292, !3}
!293 = distinct !{!293, !3}
!294 = distinct !{!294, !3}
!295 = distinct !{!295, !3}
!296 = distinct !{!296, !3}
!297 = distinct !{!297, !3}
!298 = distinct !{!298, !3}
!299 = distinct !{!299, !3}
!300 = distinct !{!300, !3}
!301 = distinct !{!301, !3}
!302 = distinct !{!302, !3}
!303 = distinct !{!303, !3}
!304 = distinct !{!304, !3}
!305 = distinct !{!305, !3}
!306 = distinct !{!306, !3}
!307 = distinct !{!307, !3}
!308 = distinct !{!308, !3}
!309 = distinct !{!309, !3}
!310 = distinct !{!310, !3}
!311 = !{!312, !47, i64 0}
!312 = !{!"", !47, i64 0, !5, i64 8}
!313 = !{!312, !5, i64 8}
!314 = distinct !{!314, !3}
!315 = distinct !{!315, !3}
!316 = distinct !{!316, !3}
!317 = distinct !{!317, !3}
!318 = !{!319, !47, i64 0}
!319 = !{!"", !47, i64 0, !50, i64 8}
!320 = !{!319, !50, i64 8}
!321 = distinct !{!321, !3}
!322 = distinct !{!322, !3}
!323 = distinct !{!323, !3}
!324 = distinct !{!324, !3}
!325 = !{!326, !47, i64 0}
!326 = !{!"", !47, i64 0, !50, i64 8, !50, i64 12}
!327 = !{!326, !50, i64 8}
!328 = !{!326, !50, i64 12}
!329 = distinct !{!329, !3}
!330 = distinct !{!330, !3}
!331 = distinct !{!331, !3}
!332 = distinct !{!332, !3}
!333 = distinct !{!333, !3}
!334 = distinct !{!334, !3}
!335 = distinct !{!335, !3}
!336 = distinct !{!336, !3}
!337 = distinct !{!337, !3}
!338 = distinct !{!338, !3}
!339 = distinct !{!339, !3}
!340 = distinct !{!340, !3}
!341 = distinct !{!341, !3}
!342 = distinct !{!342, !3}
!343 = distinct !{!343, !3}
!344 = distinct !{!344, !3}
!345 = distinct !{!345, !3}
!346 = distinct !{!346, !3}
!347 = distinct !{!347, !3}
!348 = distinct !{!348, !3}
!349 = distinct !{!349, !3}
!350 = distinct !{!350, !3}
!351 = distinct !{!351, !3}
!352 = distinct !{!352, !3}
!353 = distinct !{!353, !3}
!354 = distinct !{!354, !3}
!355 = distinct !{!355, !3}
!356 = distinct !{!356, !3}
!357 = distinct !{!357, !3}
!358 = distinct !{!358, !3}
!359 = distinct !{!359, !3}
!360 = !{!44, !46, i64 0}
!361 = !{!44, !46, i64 8}
!362 = !{!44, !46, i64 16}
!363 = !{!44, !46, i64 24}
!364 = !{!365, !50, i64 0}
!365 = !{!"", !50, i64 0}
