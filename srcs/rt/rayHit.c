#include "rt.h"

t_vector			vDirCamToPoint(t_camera cam, double x, double y)
{
	t_vector		pointOnCanvas;
	t_vector		vDir;

	pointOnCanvas = ft_vsub(ft_vmul(cam.direction[1], y * 0.01),
				ft_vadd(cam.vpUpLeft, ft_vmul(cam.direction[0], x * 0.01)));
	vDir = ft_vsub(cam.origin, pointOnCanvas);
	return (ft_normalize(vDir));
}

double				distToHit(t_object obj, t_vector o, t_vector dir)
{
	double			(*func[NBR_SHAPE])(t_object, t_vector, t_vector) = {
		sphere, plan, cylindre, cone};

	if (obj.type == -1)
		return (-1);
	return (func[obj.type](obj, o, dir));
}

static void			addClosestObj(t_rayHit *hit, t_object obj, double dist, double maxDist)
{
	if (dist <= 0 || dist >= maxDist)
		return ;
	if (dist > 0 && (!hit->distance || dist < hit->distance))
	{
		hit->distance = dist;
		hit->obj = obj;
	}
}

static t_vector		hitPoint(t_vector o, t_vector dir, double dist)
{
	return (ft_vadd(o, ft_vmul(dir, dist)));
}

void				getHitData(t_rayHit *hit, t_vector o, t_vector dir)
{
	if (!hit->distance)
	{
		hit->color = g_env.scene.background;
		return;
	}
	hit->point = hitPoint(o, dir, hit->distance);
	hit->normal = normal(hit->point, hit->obj);
	for (int i = 2; i >= 0; i--)
		hit->normal = ft_vrotate(hit->normal, hit->obj.matRotInv[i]);
	hit->point = ft_vadd(ft_vmul(hit->normal, 1e-5), hit->point);

}

t_rayHit			rayCast(t_vector o, t_vector dir, double maxDist)
{
	t_rayHit		hit;
	t_object		*currentObj;
	double			currentDist;

	ft_bzero(&hit, sizeof(t_rayHit));
	currentObj = g_env.object;
	while (currentObj)
	{
		currentDist = distToHit(*currentObj, o, dir);
		addClosestObj(&hit, *currentObj, currentDist, maxDist);
		currentObj = currentObj->next;
	}
	getHitData(&hit, o, dir);
	return (hit);
}
