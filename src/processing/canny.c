#include "canny.h"

#define PI 3.14159265358979323846
#define GAUSSIAN_CUT_OFF .005
#define MAGNITUDE_LIMIT 1000.
#define MAGNITUDE_SCALE 100.
#define NORMALIZE_CONTRAST 0

void normalizeContrast(Canny_filter *cf);
void init_array(Canny_filter *cf);
void read_lumi(Canny_filter *cf);
void read_lumi(Canny_filter *cf);
float gaussian(float x, float sig);
float dist(float x, float y);
float absf(float x);
void comp_gradient(Canny_filter *cf, float k_raduis, float k_width);
void follow(Canny_filter *cf, int xi, int yi, int ii, int t);
void perf_hysteresis(Canny_filter *cf, int low, int hight);
void comp_edges(Canny_filter *cf);

void canny_free(Canny_filter *cf);


void init_array(Canny_filter *cf)
{
    int size = cf->w * cf->h;
    cf->data = (int *) malloc(size * sizeof(int));
    cf->lumin = (int *) malloc(size * sizeof(int));
    cf->magnitude = (int *) malloc(size * sizeof(int));
    cf->dir = (float *) malloc(size * sizeof(float));
    cf->x_conv = (float *) malloc(size * sizeof(float));
    cf->y_conv = (float *) malloc(size * sizeof(float));
    cf->x_gradient = (float *) malloc(size * sizeof(float));
    cf->y_gradient = (float *) malloc(size * sizeof(float));
}

void canny_free(Canny_filter *cf)
{
    free(cf->data);
    free(cf->lumin);
    free(cf->magnitude);
    free(cf->dir);
    free(cf->x_conv);
    free(cf->y_conv);
    free(cf->x_gradient);
    free(cf->y_gradient);
    free(cf);
}

void read_lumi(Canny_filter *cf)
{
    int n = gdk_pixbuf_get_has_alpha(cf->image) ? 4 : 3;
    int row_size = gdk_pixbuf_get_rowstride(cf->image);
    guchar *ori = gdk_pixbuf_get_pixels(cf->image);
    for(int y = 0; y < cf->h; ++y)
        for(int x = 0; x < cf->w; ++x)
        {
            guchar *p = ori + y * row_size + x * n;
            int i = y * cf->w + x;
            int luminescence = (int) roundf((float) (.299 * p[0] + .587 * p[1] + .114 * p[2]));
            cf->data[i] = luminescence;
            cf->lumin[i] = luminescence;
        }
}

float gaussian(float x, float sig)
{
    return expf((float) (-(x * x) / (2. * sig * sig)));
}

float dist(float x, float y)
{
    return sqrtf((x * x) + (y * y));
}

float absf(float x)
{
    if(x < 0)
        return -x;
    return x;
}

void comp_gradient(Canny_filter *cf, float k_raduis, float k_width)
{
    float *kernel = (float *) malloc((size_t) (k_width * sizeof(float)));
    float *k_diff = (float *) malloc((size_t) (k_width * sizeof(float)));
    float k_div = (float) (2. * PI * k_raduis * k_raduis);
    for(int i = 0; i < k_width; ++i)
    {
        float g1 = gaussian(i, k_raduis);
        if(g1 <= GAUSSIAN_CUT_OFF && i >= 2)
            break;
        float g2 = gaussian((float) (i - .5), k_raduis);
        float g3 = gaussian((float) (i + .5), k_raduis);
        kernel[i] = (float) ((g1 + g2 + g3) / 3. / k_div);
        k_diff[i] = g3 - g2;
    }
    int init_x = (int) (k_width - 1);
    int init_y = cf->w * init_x;
    int max_x = cf->w - init_x;
    int max_y = cf->w * (cf->h - init_x);
    for(int x = init_x; x < max_x; ++x)
        for(int y = init_y; y < max_y; y += cf->w)
        {
            int i = x + y;
            float sum_x = cf->data[i] * kernel[0];
            float sum_y = sum_x;
            int xo = 1;
            int yo = cf->w;
            for(; xo < k_width; )
            {
                sum_y += kernel[xo] * (cf->data[i - yo] + cf->data[i + yo]);
                sum_x += kernel[xo] * (cf->data[i - xo] + cf->data[i + xo]);
                yo += cf->w;
                ++xo;
            }
            cf->x_conv[i] = sum_x;
            cf->y_conv[i] = sum_y;
        }
    for(int x = init_x; x < max_x; ++x)
        for(int y = init_y; y < max_y; y += cf->w)
        {
            float s = (float) 0.;
            int i = x + y;
            for(int a = 1; a < k_width; ++a)
                s += k_diff[a] * (cf->y_conv[i - a] - cf->y_conv[i + a]);
            cf->x_gradient[i] = s;
        }
    for (int x = (int) k_width; x < cf->w - k_width; ++x)
        for(int y = init_y; y < max_y; y += cf->w)
        {
            float s = (float) 0.;
            int i = x + y;
            int yo = cf->w;
            for(int a = 1; a < k_width; ++a)
            {
                s += k_diff[a] * (cf->x_conv[i - yo] - cf->x_conv[i + yo]);
                yo += cf->w;
            }
            cf->y_gradient[i] = s;
        }
    init_x = (int) k_width;
    init_y = (int) (cf->w - k_width);
    max_x = (int) (cf->w * k_width);
    max_y = (int) (cf->w * (cf->h - k_width));
    for(int x = init_x; x < max_x; ++x)
        for(int y = init_y; y < max_y; y += cf->w)
        {
            int i = x + y;
            int in = i - cf->w;
            int is = i + cf->w;
            int iw = i - 1;
            int ie = i + 1;
            int inw = in - 1;
            int ine = in + 1;
            int isw = is - 1;
            int ise = is + 1;
            float xg = cf->x_gradient[i];
            float yg = cf->y_gradient[i];
            float gm = dist(xg, yg);
            float mn = dist(cf->x_gradient[in], cf->y_gradient[in]);
            float ms = dist(cf->x_gradient[is], cf->y_gradient[is]);
            float mw = dist(cf->x_gradient[iw], cf->y_gradient[iw]);
            float me = dist(cf->x_gradient[ie], cf->y_gradient[ie]);
            float mnw = dist(cf->x_gradient[inw], cf->y_gradient[inw]);
            float mne = dist(cf->x_gradient[ine], cf->y_gradient[ine]);
            float msw = dist(cf->x_gradient[isw], cf->y_gradient[isw]);
            float mse = dist(cf->x_gradient[ise], cf->y_gradient[ise]);
            float t;
            if(xg * yg <= 0.
               ? absf(xg) >= absf(yg)
               ? (t = absf(xg * gm)) >= absf(yg * mne - (xg + yg) * me)
               && t > absf(yg * msw - (xg + yg) * mw)
               : (t = absf(yg * gm)) >= absf(xg * mne - (yg + xg) * mn)
               && t > absf(xg * msw - (yg + xg) * ms)
               : absf(xg) >= absf(yg)
               ? (t = absf(xg * gm)) >= absf(yg * mse + (xg - yg) * me)
               && t > absf(yg * mnw + (xg - yg) * mw)
               : (t = absf(yg * gm)) >= absf(xg * mse + (yg - xg) * ms)
               && t > absf(xg * mnw + (yg - xg) * mn))
            {
                if(gm >= MAGNITUDE_LIMIT)
                    cf->magnitude[i] = (int) (MAGNITUDE_LIMIT * MAGNITUDE_SCALE);
                else
                    cf->magnitude[i] = (int) (MAGNITUDE_SCALE * gm);
                cf->dir[i] = atan2f(yg, xg);
            }
            else
            {
                cf->magnitude[i] = 0;
                cf->dir[i] = NAN;
            }
        }
}

void follow(Canny_filter *cf, int xi, int yi, int ii, int t)
{
    int xa = xi == 0 ? 0 : xi - 1;
    int ya = yi == 0 ? 0 : yi - 1;
    int xb = xi == cf->w - 1 ? xi : xi - 1;
    int yb = yi == cf->h - 1 ? yi : yi - 1;
    cf->data[ii] = cf->magnitude[ii];
    for(int x = xa; x <= xb; ++x)
        for(int y = ya; y <= yb; ++y)
        {
            int i = x + y * cf->w;
            int b = y != ya || x != xa;
            if(b && cf->data[i] == 0 && cf->magnitude[i] >= t)
            {
                follow(cf, x, y, i, t);
                return;
            }
        }
}

void perf_hysteresis(Canny_filter *cf, int low, int hight)
{
    for(int i = 0; i < cf->w * cf->h; ++i)
        cf->data[i] = 0;
    int offset = 0;
    for(int y = 0; y < cf->h; ++y)
        for(int x = 0; x < cf->w; ++x)
        {
            if(cf->data[offset] == 0 && cf->magnitude[offset] >= hight)
                follow(cf, x, y, offset, low);
            ++offset;
        }
}

void comp_edges(Canny_filter *cf)
{
    for(int i = 0; i < cf->w * cf->h; ++i)
        cf->data[i] = cf->data[i] > 0;
}

void normalizeContrast(Canny_filter *cf)
{
    int histogram[256];
    for (int i = 0; i < cf->w * cf-> h; ++i) {
        ++histogram[cf->data[i]];
    }
    int remap[256];
    int sum = 0;
    int j = 0;
    for (int i = 0; i < 256; ++i) {
        sum += histogram[i];
        int target = sum*255/(cf->w * cf->h);
        for (int k = j+1; k <=target; k++) {
            remap[k] = i;
        }
        j = target;
    }

    for (int i = 0; i < (cf->w * cf->h); i++) {
        cf->data[i] = remap[cf->data[i]];
    }
}

Canny_filter *canny(GdkPixbuf *image)
{
    Canny_filter *cf = (Canny_filter *) malloc(sizeof(Canny_filter));
    cf->image = image;
    cf->h = gdk_pixbuf_get_height(image);
    cf->w = gdk_pixbuf_get_width(image);
    init_array(cf);
    read_lumi(cf);
    if (NORMALIZE_CONTRAST)
        normalizeContrast(cf);
    float gaussianKernelRaduis = 2;
    float gaussianKernelWidth = 16;
    comp_gradient(cf, gaussianKernelRaduis, gaussianKernelWidth);
    int low = (int) roundf((float) (0.5 * MAGNITUDE_SCALE));
    int high = (int) round(1.0 * MAGNITUDE_SCALE);
    perf_hysteresis(cf, low, high);
    comp_edges(cf);
    return cf;
}
