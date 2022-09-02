компиляция в web
	библиотека
		emcc -c Platform.cpp -Os -Wall
		emcc -c Core.cpp -Os -Wall
		emcc -c Renderer.cpp -Os -Wall
		emcc -c Engine.cpp -Os -Wall
		emar rcs MiniEngine.a Core.o Platform.o Renderer.o Engine.o


bool isPointInsideTriangle(const Vec3f &vertex0, const Vec3f &vertex1, const Vec3f &vertex2, const Vec3f &pt)
{
	Vec3f u = vertex1 - vertex0;
	Vec3f v = vertex2 - vertex0;
	Vec3f w = pt - vertex0;

	float uu = u.dot(u);
	float uv = u.dot(v);
	float vv = v.dot(v);
	float wu = w.dot(u);
	float wv = w.dot(v);
	float d = uv * uv - uu * vv;

	float invD = 1 / d;
	float s = (uv * wv - vv * wu) * invD;
	if (s < 0 || s > 1)
		return false;
	float t = (uv * wu - uu * wv) * invD;
	if (t < 0 || (s + t) > 1)
		return false;

	return true;
}

bool isPointInsidePolygon(int nbVertices, const Vec3f *pnts, const Vec3f &pt)
{
	int nbTriangles = nbVertices - 2;
	for (int i = 0; i < nbTriangles; i++) {
		const Vec3f &pt0 = pnts[0];
		const Vec3f &pt1 = pnts[i + 1];
		const Vec3f &pt2 = pnts[i + 2];
		if (isPointInsideTriangle(pt0, pt1, pt2, pt))
			return true;
	}
	return false;
}




Дизеринг
	посмотреть как сделано в анарх - сделать такое же на шейдере (например https://medium.com/the-bkpt/dithered-shading-tutorial-29f57d06ac39)
	также его можно заюзать на небо


физика из cube


Кубический опенворд
	вот пример геометрии - https://images-wixmp-ed30a86b8c4ca887773594c2.wixmp.com/f/862b1eec-47a9-44b9-9915-69d8b925ab1d/d8mc9hu-5544e721-1e59-47b7-9f3b-39f41d1ee7f1.png?token=eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzdWIiOiJ1cm46YXBwOjdlMGQxODg5ODIyNjQzNzNhNWYwZDQxNWVhMGQyNmUwIiwiaXNzIjoidXJuOmFwcDo3ZTBkMTg4OTgyMjY0MzczYTVmMGQ0MTVlYTBkMjZlMCIsIm9iaiI6W1t7InBhdGgiOiJcL2ZcLzg2MmIxZWVjLTQ3YTktNDRiOS05OTE1LTY5ZDhiOTI1YWIxZFwvZDhtYzlodS01NTQ0ZTcyMS0xZTU5LTQ3YjctOWYzYi0zOWY0MWQxZWU3ZjEucG5nIn1dXSwiYXVkIjpbInVybjpzZXJ2aWNlOmZpbGUuZG93bmxvYWQiXX0.CYqZuL20ZU59njrixp6hhMw0kBItPbSW0WiLt7FVNkI