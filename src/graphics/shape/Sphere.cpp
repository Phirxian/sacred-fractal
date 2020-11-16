#include "Sphere.h"
#include <cmath>

namespace sacred
{
    namespace graphics
    {
        namespace shape
        {
            IcoSphere::IcoSphere(int recursionLevel)
                : index(0), subdivide(recursionLevel)
            {
                double t = (1.0f + std::sqrt(5.0f))/2.0f;
                osg::Vec3Array *vertex = new osg::Vec3Array;

                AddVertex(osg::Vec3(-1,  t, 0), vertex);
                AddVertex(osg::Vec3( 1,  t, 0), vertex);
                AddVertex(osg::Vec3(-1, -t, 0), vertex);
                AddVertex(osg::Vec3( 1, -t, 0), vertex);

                AddVertex(osg::Vec3(0, -1,  t), vertex);
                AddVertex(osg::Vec3(0,  1,  t), vertex);
                AddVertex(osg::Vec3(0, -1, -t), vertex);
                AddVertex(osg::Vec3(0,  1, -t), vertex);

                AddVertex(osg::Vec3( t, 0, -1), vertex);
                AddVertex(osg::Vec3( t, 0,  1), vertex);
                AddVertex(osg::Vec3(-t, 0, -1), vertex);
                AddVertex(osg::Vec3(-t, 0,  1), vertex);

                setVertexArray(vertex);

                // create 20 triangles of the icosahedron
                vecTriangle = new osg::Vec3Array;

                // 5 faces around point 0
                vecTriangle->push_back(osg::Vec3(0, 11,  5));
                vecTriangle->push_back(osg::Vec3(0,  5,  1));
                vecTriangle->push_back(osg::Vec3(0,  1,  7));
                vecTriangle->push_back(osg::Vec3(0,  7, 10));
                vecTriangle->push_back(osg::Vec3(0, 10, 11));

                // 5 adjacent faces
                vecTriangle->push_back(osg::Vec3( 1,  5,  9));
                vecTriangle->push_back(osg::Vec3( 5, 11,  4));
                vecTriangle->push_back(osg::Vec3(11, 10,  2));
                vecTriangle->push_back(osg::Vec3(10,  7,  6));
                vecTriangle->push_back(osg::Vec3( 7,  1,  8));

                // 5 faces around point 3
                vecTriangle->push_back(osg::Vec3(3, 9, 4));
                vecTriangle->push_back(osg::Vec3(3, 4, 2));
                vecTriangle->push_back(osg::Vec3(3, 2, 6));
                vecTriangle->push_back(osg::Vec3(3, 6, 8));
                vecTriangle->push_back(osg::Vec3(3, 8, 9));

                // 5 adjacent faces
                vecTriangle->push_back(osg::Vec3(4, 9,  5));
                vecTriangle->push_back(osg::Vec3(2, 4, 11));
                vecTriangle->push_back(osg::Vec3(6, 2, 10));
                vecTriangle->push_back(osg::Vec3(8, 6,  7));
                vecTriangle->push_back(osg::Vec3(9, 8,  1));

                buildVertices();
//                buildColorMapping();
                buildUvMapping();
            }

            void IcoSphere::buildVertices()
            {
                for(int i=0; i<subdivide; ++i)
                {
                    osg::Vec3Array *vecTriangleNew = new osg::Vec3Array;

                    for(int j=0; j<vecTriangle->size(); ++j)
                    {
                        int a = GetMiddlePoint(vecTriangle->at(j).x(), vecTriangle->at(j).y());
                        int b = GetMiddlePoint(vecTriangle->at(j).y(), vecTriangle->at(j).z());
                        int c = GetMiddlePoint(vecTriangle->at(j).z(), vecTriangle->at(j).x());

                        vecTriangleNew->push_back(osg::Vec3(vecTriangle->at(j).x(), a, c));
                        vecTriangleNew->push_back(osg::Vec3(vecTriangle->at(j).y(), b, a));
                        vecTriangleNew->push_back(osg::Vec3(vecTriangle->at(j).z(), c, b));

                        vecTriangleNew->push_back(osg::Vec3(a, b, c));
                    }

                    vecTriangle = vecTriangleNew;
                }

                // add triangle to mesh
                for(int i=0; i<vecTriangle->size(); ++i)
                    DrawTriangle(vecTriangle->at(i).x(), vecTriangle->at(i).y(), vecTriangle->at(i).z());
            }

            void IcoSphere::buildColorMapping()
            {
                osg::Vec4Array *colors = new osg::Vec4Array;

                for(int i=0; i<index; ++i)
                    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f));

                setColorArray(colors);
                setColorBinding(osg::Geometry::BIND_PER_VERTEX);
            }

            void IcoSphere::buildUvMapping()
            {
                osg::Vec2Array *coord = new osg::Vec2Array;

                for(int i = 0; i < index; i++)
                {
                    osg::Vec3 v = ((osg::Vec3*)vecTriangle->getDataPointer())[i];

                    osg::Vec2 textureCoordinates;

                    textureCoordinates.x() = atan2(v.x(), v.z()) / (-2.f * osg::PI);
                    textureCoordinates.y() = asin(v.y()) / osg::PI + 0.5f;

                    coord->push_back(textureCoordinates);
                }

//                int tt = tri * 3;
//                uv[0 * tt + 0].x = 0.875f;
//                uv[1 * tt + 0].x = 0.875f;
//                uv[2 * tt + 0].x = 0.125f;
//                uv[3 * tt + 0].x = 0.125f;
//                uv[4 * tt + 0].x = 0.625f;
//                uv[5 * tt + 0].x = 0.375f;
//                uv[6 * tt + 0].x = 0.375f;
//                uv[7 * tt + 0].x = 0.625f;

                setTexCoordArray(0, coord);
            }

            void IcoSphere::DrawTriangle(int point0, int point1, int point2)
            {
                osg::ref_ptr<osg::DrawElementsUInt> triangle = new osg::DrawElementsUInt(osg::PrimitiveSet::TRIANGLES, 0);
                triangle->push_back(point2);
                triangle->push_back(point0);
                triangle->push_back(point1);
                addPrimitiveSet(triangle.get());
            }

            void IcoSphere::Normalize(osg::Vec3 &pVec)
            {
                double length = std::sqrt(pVec.x()*pVec.x() + pVec.y()*pVec.y() + pVec.z()*pVec.z());
                pVec.x() /= length;
                pVec.y() /= length;
                pVec.z() /= length;
            }

            int IcoSphere::AddVertex(osg::Vec3 pos, osg::Vec3Array *vertex)
            {
                Normalize(pos);
                vertex->push_back(pos);
                return index++;
            }

            int IcoSphere::GetMiddlePoint(int point1, int point2)
            {
                int smallerIndex = point1 < point2 ? point1 : point2;
                int greaterIndex = point1 > point2 ? point1 : point2;
                long key = (smallerIndex << 16) + greaterIndex;
                std::map<long, int>::iterator it = middlePointIndexCache.find(key);

                if(middlePointIndexCache.end() != it)
                    return it->second;

                osg::Vec3Array *pVecPoint = dynamic_cast<osg::Vec3Array *>(getVertexArray());

                osg::Vec3 vecMiddle(
                    (pVecPoint->at(point1).x() + pVecPoint->at(point2).x())/2,
                    (pVecPoint->at(point1).y() + pVecPoint->at(point2).y())/2,
                    (pVecPoint->at(point1).z() + pVecPoint->at(point2).z())/2
                );

                int i = AddVertex(vecMiddle, pVecPoint);
                setVertexArray(pVecPoint);
                middlePointIndexCache.insert(std::pair<long, int>(key, i));

                return i;
            }
        }
    }
}
