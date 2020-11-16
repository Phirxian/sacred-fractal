#include <osg/Geometry>

namespace sacred
{
    namespace graphics
    {
        namespace shape
        {
            class IcoSphere : public osg::Geometry
            {
                public:
                    IcoSphere(int subdivide);

                protected:
                    void buildVertices();
                    void buildColorMapping();
                    void buildUvMapping();

                private:
                    int  AddVertex(osg::Vec3 value, osg::Vec3Array *vertex);
                    void DrawTriangle(int point0, int point1, int point2);
                    void Normalize(osg::Vec3 &pVec);
                    int  GetMiddlePoint(int point1, int point2);

                private:
                    int index;
                    int subdivide;
                    osg::Vec3Array *vecTriangle;
                    std::map<long, int> middlePointIndexCache;
            };
        }
    }
}
