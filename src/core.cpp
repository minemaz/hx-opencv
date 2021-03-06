#include <core/core_c.h>
#include <core/core.hpp>

#include "utils.h"
#define GETPROP(N, M, I) PGETPROP(core, N, M, I)
#define SETPROP(N, M, I) PSETPROP(core, N, M, I)
#define PROP(N, M, I) PPROP(core, N, M, I)


//
// CvPoint
// CvPoint2D32f
// CvPoint2D64f
//
#define INIT_POINT2D(N, F) \
    DEFINE_KIND(k_##N); \
    value hx_cv_core_##N(value x, value y) { \
        Cv##N* ptr = new Cv##N; \
        ptr->x = val_get<F>(x); \
        ptr->y = val_get<F>(y); \
        value v = alloc_abstract(k_##N, ptr); \
        val_gc(v, finaliser<Cv##N>); \
        return v; \
    } \
    DEFINE_PRIM(hx_cv_core_##N, 2); \
    PROP(N, x, F); \
    PROP(N, y, F); \
    PDEFINE_CONVERT_GENERIC(core, N)
INIT_POINT2D(Point,      int   );
INIT_POINT2D(Point2D32f, double);
INIT_POINT2D(Point2D64f, double);



//
// CvPoint3D32f
// CvPoint3D64f
//
#define INIT_POINT3D(N) \
    DEFINE_KIND(k_##N); \
    value hx_cv_core_##N(value x, value y, value z) { \
        Cv##N* ptr = new Cv##N; \
        ptr->x = val_get<double>(x); \
        ptr->y = val_get<double>(y); \
        ptr->z = val_get<double>(z); \
        value v = alloc_abstract(k_##N, ptr); \
        val_gc(v, finaliser<Cv##N>); \
        return v; \
    } \
    DEFINE_PRIM(hx_cv_core_##N, 3); \
    PROP(N, x, double); \
    PROP(N, y, double); \
    PROP(N, z, double)

INIT_POINT3D(Point3D32f);
INIT_POINT3D(Point3D64f);



//
// CvSize
// CvSize2D32f
//
#define INIT_SIZE2D(N, F) \
    DEFINE_KIND(k_##N); \
    value hx_cv_core_##N(value width, value height) { \
        Cv##N* ptr = new Cv##N; \
        ptr->width  = val_get<F>(width); \
        ptr->height = val_get<F>(height); \
        value v = alloc_abstract(k_##N, ptr); \
        val_gc(v, finaliser<Cv##N>); \
        return v; \
    } \
    DEFINE_PRIM(hx_cv_core_##N, 2); \
    PROP(N, width,  F); \
    PROP(N, height, F)

INIT_SIZE2D(Size,      int   );
INIT_SIZE2D(Size2D32f, double);

PDEFINE_CONVERT_GENERIC(core, Size);
value cv_Size(const CvSize& s) {
    CvSize* ptr = new CvSize;
    ptr->width = s.width;
    ptr->height = s.height;
    return CONVERT(core, Size, ptr);
}



//
// CvRect
//
DEFINE_KIND(k_Rect);
value hx_cv_core_Rect(value x, value y, value width, value height) {
    CvRect* ptr = new CvRect;
    ptr->x      = val_get<int>(x);
    ptr->y      = val_get<int>(y);
    ptr->width  = val_get<int>(width);
    ptr->height = val_get<int>(height);
    value v = alloc_abstract(k_Rect, ptr);
    val_gc(v, finaliser<CvRect>);
    return v;
}
DEFINE_PRIM(hx_cv_core_Rect, 4);
PROP(Rect, x,      int);
PROP(Rect, y,      int);
PROP(Rect, width,  int);
PROP(Rect, height, int);



//
// CvScalar
//
DEFINE_KIND(k_Scalar);
PDEFINE_CONVERT_GENERIC(core, Scalar);
value hx_cv_core_Scalar(value v0, value v1, value v2, value v3) {
    CvScalar* ptr = new CvScalar;
    ptr->val[0] = val_get<double>(v0);
    ptr->val[1] = val_get<double>(v1);
    ptr->val[2] = val_get<double>(v2);
    ptr->val[3] = val_get<double>(v3);
    return CONVERT(core, Scalar, ptr);
}
value cv_Scalar(const CvScalar& s) {
    CvScalar* ptr = new CvScalar;
    ptr->val[0] = s.val[0];
    ptr->val[1] = s.val[1];
    ptr->val[2] = s.val[2];
    ptr->val[3] = s.val[3];
    return CONVERT(core, Scalar, ptr);
}
value hx_cv_core_Scalar_get_i(value v, value i) {
    val_check_kind(v, k_Scalar);
    CvScalar* ptr = (CvScalar*)val_data(v);
    return alloc<float>(ptr->val[val_get<int>(i)]);
}
value hx_cv_core_Scalar_set_i(value v, value i, value x) {
    val_check_kind(v, k_Scalar);
    CvScalar* ptr = (CvScalar*)val_data(v);
    return alloc<float>(ptr->val[val_get<int>(i)] = val_get<double>(x));
}
DEFINE_PRIM(hx_cv_core_Scalar,       4);
DEFINE_PRIM(hx_cv_core_Scalar_get_i, 2);
DEFINE_PRIM(hx_cv_core_Scalar_set_i, 3);


//
// CvTermCriteria
// cvCheckTermCriteria
//
DEFINE_KIND(k_TermCriteria);
value hx_cv_core_TermCriteria(value type, value max_iter, value epsilon) {
    CvTermCriteria* ptr = new CvTermCriteria;
    ptr->type     = val_get<int>(type);
    ptr->max_iter = val_get<int>(max_iter);
    ptr->epsilon  = val_get<double>(epsilon);
    value v = alloc_abstract(k_TermCriteria, ptr);
    val_gc(v, finaliser<CvTermCriteria>);
    return v;
}
value hx_cv_core_checkTermCriteria(value criteria, value default_eps, value default_max_iters) {
    val_check_kind(criteria, k_TermCriteria);
    CvTermCriteria* ptr = (CvTermCriteria*)val_data(criteria);
    CvTermCriteria nxt = cvCheckTermCriteria(*ptr, val_get<double>(default_eps), val_get<int>(default_max_iters));
    return hx_cv_core_TermCriteria(alloc<int>(nxt.type), alloc<int>(nxt.max_iter), alloc<double>(nxt.epsilon));
}
DEFINE_PRIM(hx_cv_core_TermCriteria,      3);
DEFINE_PRIM(hx_cv_core_checkTermCriteria, 3);
PROP(TermCriteria, type, int);
PROP(TermCriteria, max_iter, int);
PROP(TermCriteria, epsilon, double);



//
// CvMat
// cvCreateMat
// cvCreateMatHeader
// cvmGet
// cvmSet
//
DEFINE_KIND(k_Mat);
static void finalise_Mat(value v) {
    CvMat* ptr = (CvMat*)val_data(v);
    cvReleaseMat(&ptr);
}
PDEFINE_CONVERT(core, Mat, finalise_Mat);
value hx_cv_core_createMat(value rows, value cols, value type) {
    return CONVERT(core, Mat, cvCreateMat(val_get<int>(rows), val_get<int>(cols), val_get<int>(type)));
}
value hx_cv_core_createMatHeader(value rows, value cols, value type) {
    return CONVERT(core, Mat, cvCreateMatHeader(val_get<int>(rows), val_get<int>(cols), val_get<int>(type)));
}
value hx_cv_core_cloneMat(value mat) {
    val_check_kind(mat, k_Mat);
    return CONVERT(core, Mat, cvCloneMat((CvMat*)val_data(mat)));
}
value hx_cv_core_mGet(value mat, value row, value col) {
    val_check_kind(mat, k_Mat);
    CvMat* ptr = (CvMat*)val_data(mat);
    return alloc<double>(cvmGet(ptr, val_get<int>(row), val_get<int>(col)));
}
value hx_cv_core_mSet(value mat, value row, value col, value value) {
    val_check_kind(mat, k_Mat);
    CvMat* ptr = (CvMat*)val_data(mat);
    cvmSet(ptr, val_get<int>(row), val_get<int>(col), val_get<double>(value));
    return value;
}
#define MATVAL(N, P, F) \
    value hx_cv_core_mat_##N##_get(value mat, value i) { \
        val_check_kind(mat, k_Mat); \
        CvMat* ptr = (CvMat*)val_data(mat); \
        return alloc<F>(ptr->data.P[val_get<int>(i)]); \
    } \
    value hx_cv_core_mat_##N##_set(value mat, value i, value v) { \
        val_check_kind(mat, k_Mat); \
        CvMat* ptr = (CvMat*)val_data(mat); \
        return alloc<F>(ptr->data.P[val_get<int>(i)] = val_get<F>(v)); \
    } \
    DEFINE_PRIM(hx_cv_core_mat_##N##_get, 2); \
    DEFINE_PRIM(hx_cv_core_mat_##N##_set, 3)
DEFINE_PRIM(hx_cv_core_createMat,       3);
DEFINE_PRIM(hx_cv_core_createMatHeader, 3);
DEFINE_PRIM(hx_cv_core_cloneMat,        1);
DEFINE_PRIM(hx_cv_core_mGet,            3);
DEFINE_PRIM(hx_cv_core_mSet,            4);
GETPROP(Mat, type, int);
GETPROP(Mat, step, int);
GETPROP(Mat, rows, int);
GETPROP(Mat, cols, int);
MATVAL(uchar,  ptr, int   );
MATVAL(short,  s,   int   );
MATVAL(int,    i,   int   );
MATVAL(float,  fl,  double);
MATVAL(double, db,  double);

DECLARE_KIND(k_CArray);
DEFINE_KIND(k_CArray);

value hx_cv_core_Mat_get_ptr(value image) {
    CvMat* ptr = (CvMat*)val_data(image);
    value ret = alloc_abstract(k_CArray, ptr->data.ptr);
    return ret;
}
DEFINE_PRIM(hx_cv_core_Mat_get_ptr, 1);


struct Vector {
    void* dat;
    unsigned int size;
};
value hx_gl_vector_len(value b) {
    Vector* v = (Vector*)val_data(b);
    return alloc<int>(v->size);
}
DEFINE_PRIM(hx_gl_vector_len, 1)

DECLARE_KIND(k_Vector);
DEFINE_KIND(k_Vector);



// For ogl
value hx_cv_core_Mat_get_raw(value image) {
    val_check_kind(image, k_Mat);
    CvMat* ptr = (CvMat*)val_data(image);
    int size = ptr->rows*ptr->step;
    Vector* ret = new Vector;
    ret->dat = ptr->data.ptr;
    ret->size = size;
    value v = alloc_abstract(k_Vector, ret);
    return v;
}
DEFINE_PRIM(hx_cv_core_Mat_get_raw, 1);



//
// IplImage
// cvCreateImage
// cvCreateImageHeader
// cvCloneImage
//
DEFINE_KIND(k_Image);
static void finalise_Image(value v) {
    IplImage* ptr = (IplImage*)val_data(v);
    cvReleaseImage(&ptr);
}
GDEFINE_CONVERT(Ipl, core, Image, finalise_Image);
value hx_cv_core_createImage(value size, value depth, value channels) {
    val_check_kind(size, k_Size);
    return CONVERT(core, Image, cvCreateImage(*(CvSize*)val_data(size), val_get<int>(depth), val_get<int>(channels)));
}
value hx_cv_core_createImageHeader(value size, value depth, value channels) {
    val_check_kind(size, k_Size);
    return CONVERT(core, Image, cvCreateImageHeader(*(CvSize*)val_data(size), val_get<int>(depth), val_get<int>(channels)));
}
value hx_cv_core_cloneImage(value image) {
    val_check_kind(image, k_Image);
    return CONVERT(core, Image, cvCloneImage((IplImage*)val_data(image)));
}
DEFINE_PRIM(hx_cv_core_createImage,       3);
DEFINE_PRIM(hx_cv_core_createImageHeader, 3);
DEFINE_PRIM(hx_cv_core_cloneImage,        1);
GGETPROP(core, Ipl, Image, nChannels, int);
GGETPROP(core, Ipl, Image, depth,     int);
GGETPROP(core, Ipl, Image, dataOrder, int);
GGETPROP(core, Ipl, Image, origin,    int);
GGETPROP(core, Ipl, Image, width,     int);
GGETPROP(core, Ipl, Image, height,    int);
GGETPROP(core, Ipl, Image, imageSize, int);
GGETPROP(core, Ipl, Image, widthStep, int);

// For ogl
value hx_cv_core_Image_get_raw(value image) {
    val_check_kind(image, k_Image);
    IplImage* ptr = (IplImage*)val_data(image);
    int size = ptr->imageSize;
    Vector* ret = new Vector;
    ret->dat = ptr->imageData;
    ret->size = size;
    value v = alloc_abstract(k_Vector, ret);
    return v;
}
DEFINE_PRIM(hx_cv_core_Image_get_raw, 1);


//
// cvAbsDiff
// cvAbsDiffS
// cvAdd
// cvAddS
// cvAddWeighted
// cvAnd
// cvAndS
// cvAvg
// cvAvgSdv
// cvCartToPolar
// cvCmp
// cvCmpS
// cvConvertScale
// cvConvertScaleAbs
// cvCopy
// cvCountNonZero
// cvCreateData
// cvDCT
// cvDFT
// cvDet
// cvDiv
// cvDotProduct
// cvGet1D
// cvGet2D
// cvGet3D
// cvGetND
// cvGetCols
// cvGetDiag
// cvGetElemType
// cvGetReal1D
// cvGetReal2D
// cvGetReal3D
// cvGetRealND
// cvGetRows
// cvGetSize
// cvGetSubRect
// cvMul
// cvScaleAdd
// cvSet
//
value hx_cv_core_absDiff(value src1, value src2, value dst) {
    cvAbsDiff(val_data(src1), val_data(src2), val_data(dst));
    return val_null;
}
value hx_cv_core_absDiffS(value src, value dst, value _value) {
    val_check_kind(_value, k_Scalar);
    cvAbsDiffS(val_data(src), val_data(dst), *(CvScalar*)val_data(_value));
    return val_null;
}
value hx_cv_core_add(value src1, value src2, value dst, value mask) {
    cvAdd(val_data(src1), val_data(src2), val_data(dst), val_data(mask));
    return val_null;
}
value hx_cv_core_addS(value src, value _value, value dst, value mask) {
    val_check_kind(_value, k_Scalar);
    cvAddS(val_data(src), *(CvScalar*)val_data(_value), val_data(dst), val_data(mask));
    return val_null;
}
value hx_cv_core_addWeighted(value* args, int nargs) {
    if (nargs != 6) neko_error();
    value src1  = args[0];
    value alpha = args[1];
    value src2  = args[2];
    value beta  = args[3];
    value gamma = args[4];
    value dst   = args[5];

    cvAddWeighted(val_data(src1), val_get<double>(alpha), val_data(src2), val_get<double>(beta), val_get<double>(gamma), val_data(dst));
    return val_null;
}
value hx_cv_core_and(value src1, value src2, value dst, value mask) {
    cvAnd(val_data(src1), val_data(src1), val_data(dst), val_data(mask));
    return val_null;
}
value hx_cv_core_andS(value src, value _value, value dst, value mask) {
    val_check_kind(_value, k_Scalar);
    cvAndS(val_data(src), *(CvScalar*)val_data(_value), val_data(dst), val_data(mask));
    return val_null;
}
value hx_cv_core_avg(value arr, value mask) {
    return cv_Scalar(cvAvg(val_data(arr), val_data(mask)));
}
value hx_cv_core_avgSdv(value arr, value mean, value stdDev, value mask) {
    val_check_kind(mean, k_Scalar);
    val_check_kind(stdDev, k_Scalar);
    cvAvgSdv(val_data(arr), (CvScalar*)val_data(mean), (CvScalar*)val_data(stdDev), val_data(mask));
    return val_null;
}
value hx_cv_core_cartToPolar(value x, value y, value magnitude, value angle, value angleInDegrees) {
    cvCartToPolar(val_data(x), val_data(y), val_data(magnitude), val_data(angle), val_get<bool>(angleInDegrees));
    return val_null;
}
value hx_cv_core_cmp(value src1, value src2, value dst, value cmpOp) {
    cvCmp(val_data(src1), val_data(src2), val_data(dst), val_get<int>(cmpOp));
    return val_null;
}
value hx_cv_core_cmpS(value src, value _value, value dst, value cmpOp) {
    cvCmpS(val_data(src), val_get<double>(_value), val_data(dst), val_get<int>(cmpOp));
    return val_null;
}
value hx_cv_core_convertScale(value src, value dst, value scale, value shift) {
    cvConvertScale(val_data(src), val_data(dst), val_get<double>(scale), val_get<double>(shift));
    return val_null;
}
value hx_cv_core_convertScaleAbs(value src, value dst, value scale, value shift) {
    cvConvertScaleAbs(val_data(src), val_data(dst), val_get<double>(scale), val_get<double>(shift));
    return val_null;
}
value hx_cv_core_copy(value src, value dst, value mask) {
    cvCopy(val_data(src), val_data(dst), val_data(mask));
    return val_null;
}
value hx_cv_core_countNonZero(value arr) {
    return alloc<int>(cvCountNonZero(val_data(arr)));
}
value hx_cv_core_createData(value arr)  {
    cvCreateData(val_data(arr));
    return val_null;
}
value hx_cv_core_DCT(value src, value dst, value flags) {
    cvDCT(val_data(src), val_data(dst), val_get<int>(flags));
    return val_null;
}
value hx_cv_core_DFT(value src, value dst, value flags, value nonzeroRows) {
    cvDFT(val_data(src), val_data(dst), val_get<int>(flags), val_get<int>(nonzeroRows));
    return val_null;
}
value hx_cv_core_det(value mat) {
    return alloc<double>(cvDet(val_data(mat)));
}
value hx_cv_core_div(value src1, value src2, value dst, value scale) {
    cvDiv(val_data(src1), val_data(src1), val_data(dst), val_get<double>(scale));
    return val_null;
}
value hx_cv_core_dotProduct(value src1, value src2) {
    return alloc<double>(cvDotProduct(val_data(src1), val_data(src2)));
}
value hx_cv_core_get1D(value arr, value idx0) {
    return cv_Scalar(cvGet1D(val_data(arr), val_get<int>(idx0)));
}
value hx_cv_core_get2D(value arr, value idx0, value idx1) {
    return cv_Scalar(cvGet2D(val_data(arr), val_get<int>(idx0), val_get<int>(idx1)));
}
value hx_cv_core_get3D(value arr, value idx0, value idx1, value idx2) {
    return cv_Scalar(cvGet3D(val_data(arr), val_get<int>(idx0), val_get<int>(idx1), val_get<int>(idx2)));
}
value hx_cv_core_getND(value arr, value idx) {
    val_check(idx, array);
    int size = val_array_size(idx);
    int* _idx = new int[size];
    for (int i = 0; i < size; i++) _idx[i] = val_get<int>(val_array_i(idx, i));
    value ret = cv_Scalar(cvGetND(val_data(arr), _idx));
    delete[] _idx;
    return ret;
}
value hx_cv_core_getCols(value arr, value submat, value startCol, value endCol) {
    val_check_kind(submat, k_Mat);
    return CONVERT(core, Mat, cvGetCols(val_data(arr), (CvMat*)val_data(submat), val_get<int>(startCol), val_get<int>(endCol)));
}
value hx_cv_core_getDiag(value arr, value submat, value diag) {
    val_check_kind(submat, k_Mat);
    return CONVERT(core, Mat, cvGetDiag(val_data(arr), (CvMat*)val_data(submat), val_get<int>(diag)));
}
value hx_cv_core_getElemType(value arr) {
    return alloc<int>(cvGetElemType(val_data(arr)));
}
value hx_cv_core_getReal1D(value arr, value idx0) {
    return alloc<double>(cvGetReal1D(val_data(arr), val_get<int>(idx0)));
}
value hx_cv_core_getReal2D(value arr, value idx0, value idx1) {
    return alloc<double>(cvGetReal2D(val_data(arr), val_get<int>(idx0), val_get<int>(idx1)));
}
value hx_cv_core_getReal3D(value arr, value idx0, value idx1, value idx2) {
    return alloc<double>(cvGetReal3D(val_data(arr), val_get<int>(idx0), val_get<int>(idx1), val_get<int>(idx2)));
}
value hx_cv_core_getRealND(value arr, value idx) {
    val_check(idx, array);
    int size = val_array_size(idx);
    int* _idx = new int[size];
    for (int i = 0; i < size; i++) _idx[i] = val_get<int>(val_array_i(idx, i));
    value ret = alloc<double>(cvGetRealND(val_data(arr), _idx));
    delete[] _idx;
    return ret;
}
value hx_cv_core_getRows(value* args, int nargs) {
    if (nargs != 5) neko_error();
    value arr      = args[0];
    value submat   = args[1];
    value startRow = args[2];
    value endRow   = args[3];
    value deltaRow = args[4];
    val_check_kind(submat, k_Mat);
    return CONVERT(core, Mat, cvGetRows(val_data(arr), (CvMat*)val_data(submat), val_get<int>(startRow), val_get<int>(endRow)));
}
value hx_cv_core_getSize(value arr) {
    return cv_Size(cvGetSize(val_data(arr)));
}
value hx_cv_core_getSubRect(value arr, value submat, value rect) {
    val_check_kind(submat, k_Mat);
    val_check_kind(rect, k_Rect);
    return CONVERT_NOGC(core, Mat, cvGetSubRect(val_data(arr), (CvMat*)val_data(submat), *(CvRect*)val_data(rect)));
}

value hx_cv_core_magnitude(value x, value y, value mag) {
    cv::magnitude(cv::cvarrToMat(val_data(x)), cv::cvarrToMat(y), cv::cvarrToMat(mag));
    return val_null;
}
value hx_cv_core_merge(value mv, value dst) {
    CvArr* d = val_data(dst);
    int cnt = val_array_size(mv);
    CvArr* x = 0 >= cnt ? NULL : val_data(val_array_i(mv, 0));
    CvArr* y = 1 >= cnt ? NULL : val_data(val_array_i(mv, 1));
    CvArr* z = 2 >= cnt ? NULL : val_data(val_array_i(mv, 2));
    CvArr* w = 3 >= cnt ? NULL : val_data(val_array_i(mv, 3));
    cvMerge(x, y, z, w, d);
    return val_null;
}
value hx_cv_core_mul(value src1, value src2, value dst, value scale) {
    cvMul(val_data(src1), val_data(src2), val_data(dst), val_get<double>(scale));
    return val_null;
}
value hx_cv_core_phase(value x, value y, value angle, value deg) {
    cv::phase(cv::cvarrToMat(val_data(x)), cv::cvarrToMat(val_data(y)), cv::cvarrToMat(val_data(angle)), val_get<bool>(deg));
    return val_null;
}
value hx_cv_core_polarToCart(value mag, value phase, value x, value y, value angleInDegrees) {
    cvPolarToCart(val_data(mag), val_data(phase), val_data(x), val_data(y), val_get<bool>(angleInDegrees));
    return val_null;
}
value hx_cv_core_scaleAdd(value src1, value scale, value src2, value dst) {
    val_check_kind(scale, k_Scalar);
    cvScaleAdd(val_data(src1), *(CvScalar*)val_data(scale), val_data(src2), val_data(dst));
    return val_null;
}
value hx_cv_core_set(value arr, value _value, value mask) {
    val_check_kind(_value, k_Scalar);
    cvSet(val_data(arr), *(CvScalar*)val_data(_value), val_data(mask));
    return val_null;
}
value hx_cv_core_split(value src, value mv) {
    CvArr* s = val_data(src);
    int cnt = val_array_size(mv);
    CvArr* x = 0 >= cnt ? NULL : val_data(val_array_i(mv, 0));
    CvArr* y = 1 >= cnt ? NULL : val_data(val_array_i(mv, 1));
    CvArr* z = 2 >= cnt ? NULL : val_data(val_array_i(mv, 2));
    CvArr* w = 3 >= cnt ? NULL : val_data(val_array_i(mv, 3));
    cvSplit(s, x, y, z, w);
    return val_null;
}
value hx_cv_core_sub(value src1, value src2, value dst, value mask) {
    cvSub(val_data(src1), val_data(src2), val_data(dst), val_data(mask));
    return val_null;
}
DEFINE_PRIM(hx_cv_core_absDiff,         3);
DEFINE_PRIM(hx_cv_core_absDiffS,        3);
DEFINE_PRIM(hx_cv_core_add,             4);
DEFINE_PRIM(hx_cv_core_addS,            4);
DEFINE_PRIM_MULT(hx_cv_core_addWeighted);
DEFINE_PRIM(hx_cv_core_and,             4);
DEFINE_PRIM(hx_cv_core_andS,            4);
DEFINE_PRIM(hx_cv_core_avg,             2);
DEFINE_PRIM(hx_cv_core_avgSdv,          4);
DEFINE_PRIM(hx_cv_core_cartToPolar,     5);
DEFINE_PRIM(hx_cv_core_cmp,             4);
DEFINE_PRIM(hx_cv_core_cmpS,            4);
DEFINE_PRIM(hx_cv_core_convertScale,    4);
DEFINE_PRIM(hx_cv_core_convertScaleAbs, 4);
DEFINE_PRIM(hx_cv_core_copy,            3);
DEFINE_PRIM(hx_cv_core_countNonZero,    1);
DEFINE_PRIM(hx_cv_core_createData,      1);
DEFINE_PRIM(hx_cv_core_DCT,             3);
DEFINE_PRIM(hx_cv_core_DFT,             4);
DEFINE_PRIM(hx_cv_core_det,             1);
DEFINE_PRIM(hx_cv_core_div,             4);
DEFINE_PRIM(hx_cv_core_dotProduct,      2);
DEFINE_PRIM(hx_cv_core_get1D,           2);
DEFINE_PRIM(hx_cv_core_get2D,           3);
DEFINE_PRIM(hx_cv_core_get3D,           4);
DEFINE_PRIM(hx_cv_core_getND,           2);
DEFINE_PRIM(hx_cv_core_getCols,         4);
DEFINE_PRIM(hx_cv_core_getDiag,         3);
DEFINE_PRIM(hx_cv_core_getElemType,     1);
DEFINE_PRIM(hx_cv_core_getReal1D,       2);
DEFINE_PRIM(hx_cv_core_getReal2D,       3);
DEFINE_PRIM(hx_cv_core_getReal3D,       4);
DEFINE_PRIM(hx_cv_core_getRealND,       2);
DEFINE_PRIM_MULT(hx_cv_core_getRows);
DEFINE_PRIM(hx_cv_core_getSize,         1);
DEFINE_PRIM(hx_cv_core_getSubRect,      3);
DEFINE_PRIM(hx_cv_core_magnitude,       3);
DEFINE_PRIM(hx_cv_core_merge,           2);
DEFINE_PRIM(hx_cv_core_mul,             4);
DEFINE_PRIM(hx_cv_core_phase,           4);
DEFINE_PRIM(hx_cv_core_polarToCart,     5);
DEFINE_PRIM(hx_cv_core_scaleAdd,        4);
DEFINE_PRIM(hx_cv_core_set,             3);
DEFINE_PRIM(hx_cv_core_split,           2);
DEFINE_PRIM(hx_cv_core_sub,             4);



// cvCircle
// cvLine
// cvCreateFont /* derived from cvInitFont */
// cvPutText
//
DECLARE_KIND(k_Font);
DEFINE_KIND(k_Font);
value hx_cv_core_circle(value* args, int nargs) {
    if (nargs != 7) neko_error();
    value img       = args[0];
    value center    = args[1];
    value radius    = args[2];
    value color     = args[3];
    value thickness = args[4];
    value lineType  = args[5];
    value shift     = args[6];
    val_check_kind(center, k_Point);
    val_check_kind(color, k_Scalar);
    cvCircle(val_data(img), *(CvPoint*)val_data(center), val_get<int>(radius), *(CvScalar*)val_data(color), val_get<int>(thickness), val_get<int>(lineType), val_get<int>(shift));
    return val_null;
}
value hx_cv_core_line(value* args, int nargs) {
    if (nargs != 7) neko_error();
    value img       = args[0];
    value pt1       = args[1];
    value pt2       = args[2];
    value color     = args[3];
    value thickness = args[4];
    value lineType  = args[5];
    value shift     = args[6];
    val_check_kind(pt1, k_Point);
    val_check_kind(pt2, k_Point);
    val_check_kind(color, k_Scalar);
    cvLine(val_data(img), *(CvPoint*)val_data(pt1), *(CvPoint*)val_data(pt2), *(CvScalar*)val_data(color), val_get<int>(thickness), val_get<int>(lineType), val_get<int>(shift));
    return val_null;
}
value hx_cv_core_rectangle(value* args, int nargs) {
    if (nargs != 7) neko_error();
    value img       = args[0];
    value pt1       = args[1];
    value pt2       = args[2];
    value color     = args[3];
    value thickness = args[4];
    value lineType  = args[5];
    value shift     = args[6];
    val_check_kind(pt1, k_Point);
    val_check_kind(pt2, k_Point);
    val_check_kind(color, k_Scalar);
    cvRectangle(val_data(img), *(CvPoint*)val_data(pt1), *(CvPoint*)val_data(pt2), *(CvScalar*)val_data(color), val_get<int>(thickness), val_get<int>(lineType), val_get<int>(shift));
    return val_null;
}
value hx_cv_core_createFont(value* args, int nargs) {
    if (nargs != 6) neko_error();
    value fontFace  = args[0];
    value hscale    = args[1];
    value vscale    = args[2];
    value shear     = args[3];
    value thickness = args[4];
    value lineType  = args[5];
    CvFont* ptr = new CvFont;
    cvInitFont(ptr, val_get<int>(fontFace), val_get<double>(hscale), val_get<double>(vscale), val_get<double>(shear), val_get<double>(thickness), val_get<int>(lineType));
    value v = alloc_abstract(k_Font, ptr);
    val_gc(v, finaliser<CvFont>);
    return v;
}
value hx_cv_core_putText(value img, value text, value org, value font, value color) {
    val_check_kind(org, k_Point);
    val_check_kind(font, k_Font);
    val_check_kind(color, k_Scalar);
    cvPutText(val_data(img), val_get<string>(text), *(CvPoint*)val_data(org), (CvFont*)val_data(font), *(CvScalar*)val_data(color));
}
DEFINE_PRIM_MULT(hx_cv_core_circle);
DEFINE_PRIM_MULT(hx_cv_core_line);
DEFINE_PRIM_MULT(hx_cv_core_rectangle);
DEFINE_PRIM_MULT(hx_cv_core_createFont);
DEFINE_PRIM(hx_cv_core_putText, 5);


extern "C" void core_allocateKinds() {
    k_Point        = alloc_kind();
    k_Point2D32f   = alloc_kind();
    k_Point2D64f   = alloc_kind();
    k_Point3D32f   = alloc_kind();
    k_Point3D64f   = alloc_kind();
    k_Size         = alloc_kind();
    k_Size2D32f    = alloc_kind();
    k_Rect         = alloc_kind();
    k_Scalar       = alloc_kind();
    k_TermCriteria = alloc_kind();
    k_Mat          = alloc_kind();
    k_Image        = alloc_kind();
    k_Font         = alloc_kind();

    k_CArray = alloc_kind();
    k_Vector = alloc_kind();
}
