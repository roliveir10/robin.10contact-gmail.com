#include "rt.h"
#include <math.h>
#include <stdlib.h>

void				delenv(void)
{
	mlx_destroy_image(g_env.mlx.mlx, g_env.mlx.image);
	freeStruct();
	exit(1);
}

static void			initmlx(void)
{
	g_env.mlx.mlx = mlx_init();
	g_env.mlx.image = mlx_new_image(g_env.mlx.mlx, SCREENX, SCREENY);
	g_env.mlx.id = mlx_new_window(g_env.mlx.mlx, SCREENX, SCREENY, "RayTracer de Robin");
	g_env.mlx.mem_image = (unsigned int*)mlx_get_data_addr(g_env.mlx.image,
			&g_env.mlx.pix, &g_env.mlx.size_line, &g_env.mlx.endian);
}

void				addPixel(t_mlx mlx, t_vector color, int x, int y)
{
	unsigned char		rgb[3];
	unsigned int		color_rgb;
	int					offSetX;
	int					offSetY;

	offSetX = x + PIXPERUNIT;
	offSetY = y + PIXPERUNIT;
	rgb[0] = (unsigned char)(ft_clamp(pow(color.x, .454545), 0, 1) * 255);
	rgb[1] = (unsigned char)(ft_clamp(pow(color.y, .454545), 0, 1) * 255);
	rgb[2] = (unsigned char)(ft_clamp(pow(color.z, .454545), 0, 1) * 255);
	color_rgb = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	for (int i = y; i < offSetY; i++)
		for (int j = x; j < offSetX; j++)
		{
			if (j + i * SCREENX < RESOLUTION)
				mlx.mem_image[j + i * SCREENX] = color_rgb;
		}
}

t_vector			attribute_color(int color)
{
	t_vector		vcolor;
	
	vcolor.x = (double)(color >> 16 & 0xFF) / 255;
	vcolor.y = (double)(color >> 8 & 0xFF) / 255;
	vcolor.z = (double)(color & 0xFF) / 255;
	return (vcolor);
}

int				rt_manager(void)
{
	t_rayHit		hit;
	double			x;
	double			y;
	t_vector		rayDir;
	t_vector		color;

	for (int i = 0; i < SCREENY; i += PIXPERUNIT)
		for (int j = 0; j < SCREENX; j += PIXPERUNIT)
		{
			ft_bzero(&color, sizeof(t_vector));
			for (int s = 0; s < SAMPLE_RATE; s++)
			{
				y = (double)i + drand48();
				x = (double)j + drand48();
				rayDir = vDirCamToPoint(g_env.camera, x, y);
				hit = rayCast(g_env.camera.origin, rayDir, 100000);
				color = ft_vadd(color, hit.color);
			}
			color = ft_vdiv(color, SAMPLE_RATE);
			addPixel(g_env.mlx, color, j, i);

		}
	mlx_put_image_to_window(g_env.mlx.mlx, g_env.mlx.id, g_env.mlx.image, 0, 0);
	return (1);
}

int				rt_main(void)
{
	initmlx();
	initCamera(&(g_env.camera));
	initializeRotation();
	mlx_hook(g_env.mlx.id, KEYPRESS, 0, keyPress, NULL);
	mlx_hook(g_env.mlx.id, REDBUTTON, 0, rt_close, NULL);
	mlx_hook(g_env.mlx.id, MOUSEPRESS, 0, mousePress, NULL);
//	mlx_hook(g_env.mlx.id, KEYRELEASE, 0, void, void);
	rt_manager();
//	mlx_loop_hook(g_env.mlx.mlx, rt_manager, &g_env.mlx);
	mlx_loop(g_env.mlx.mlx);
	return (1);
}
