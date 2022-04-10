#include "Sphere.h"
#include "ApplicationConstants.h"
#include "Mesh.h"
#include "MeshBuilder.h"

MeshBuilder* Sphere::createMeshBuilder()
{
    // clear memory of prev arrays
    auto positions = new std::vector<glm::vec3>();
    auto normals = new std::vector<glm::vec3>();
    auto texcoords = new std::vector<glm::vec2>();

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / m_radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * Constants::PI / m_sectors;
    float stackStep = Constants::PI / m_stacks;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= m_stacks; ++i)
    {
        stackAngle = Constants::PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = m_radius * cosf(stackAngle);             // r * cos(u)
        z = m_radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) positions per stack
        // the first and last positions have same position and normal, but different tex coords
        for (int j = 0; j <= m_sectors; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            positions->push_back({ x, y, z });

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals->push_back({ nx, ny, nz });

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / m_sectors;
            t = (float)i / m_stacks;
            texcoords->push_back({ s, t });
        }
    }

    // generate CCW index list of sphere triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1
    auto indices = new std::vector<unsigned int>();
    std::vector<int> lineIndices;
    int k1, k2;
    for (int i = 0; i < m_stacks; ++i)
    {
        k1 = i * (m_sectors + 1);     // beginning of current stack
        k2 = k1 + m_sectors + 1;      // beginning of next stack

        for (int j = 0; j < m_sectors; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                indices->push_back(k1);
                indices->push_back(k2);
                indices->push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (m_stacks - 1))
            {
                indices->push_back(k1 + 1);
                indices->push_back(k2);
                indices->push_back(k2 + 1);
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }

    return &MeshBuilder::builder()
        .setPositions(*positions)
        .setNormals(*normals)
        .setTexcoords(*texcoords)
        .setIndices(*indices);
}