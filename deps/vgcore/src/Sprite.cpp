#include<virintox/gcore/Sprite.h>
#include<virintox/gcore/Shader.h>



Graphics::Sprite::Sprite(){}

Graphics::Sprite::Sprite(const glm::vec2 &Pos, float scale, Texture* Tex) {
    TexPtr = Tex;
    VCount = 6;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    std::vector<float> vertices{
            Pos.x                         ,Pos.y,
                0.0f                      ,0.0f,
            Pos.x + TexPtr->size.x * scale,Pos.y,
                1.0f                      ,0.0f,
            Pos.x                         ,Pos.y + TexPtr->size.y * scale,
                0.0f                      ,1.0f,

            Pos.x + TexPtr->size.x * scale,Pos.y,
                1.0f                      ,0.0f,
            Pos.x + TexPtr->size.x * scale,Pos.y + TexPtr->size.y * scale,
                1.0f                      ,1.0f,
            Pos.x                         ,Pos.y + TexPtr->size.y * scale,
                0.0f                      ,1.0f
    };
    unsigned int indices[] = {
            0,1,2,
            3,4,5
    };

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);


    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Graphics::Sprite::Sprite(const glm::mat3x2 &Veticies, const glm::mat3x2 &TexCoords, Texture* Tex) {
    TexPtr = Tex;
    VCount = 3;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    std::vector<float> vertices{
            Veticies[0].x,Veticies[0].y,TexCoords[0].x / TexPtr->size.x,TexCoords[0].y / TexPtr->size.y,
            Veticies[1].x,Veticies[1].y,TexCoords[1].x / TexPtr->size.x,TexCoords[1].y / TexPtr->size.y,
            Veticies[2].x,Veticies[2].y,TexCoords[2].x / TexPtr->size.x,TexCoords[2].y / TexPtr->size.y
    };
    unsigned int indices[] = {
            0,1,2
    };

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);


    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Graphics::Sprite::Draw() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Graphics::GetDefaultShader2D().Use();
    TexPtr->SetActive();

    glDrawElements(GL_TRIANGLES, VCount, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Graphics::Sprite::Update(const glm::vec2 &Pos, float scale) const{
    std::vector<float> vertices{
            Pos.x                         ,Pos.y,
            0.0f                      ,0.0f,
            Pos.x + TexPtr->size.x * scale,Pos.y,
            1.0f                      ,0.0f,
            Pos.x                         ,Pos.y + TexPtr->size.y * scale,
            0.0f                      ,1.0f,

            Pos.x + TexPtr->size.x * scale,Pos.y,
            1.0f                      ,0.0f,
            Pos.x + TexPtr->size.x * scale,Pos.y + TexPtr->size.y * scale,
            1.0f                      ,1.0f,
            Pos.x                         ,Pos.y + TexPtr->size.y * scale,
            0.0f                      ,1.0f
    };
    unsigned int indices[] = {
            0,1,2,
            3,4,5
    };

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Graphics::Sprite::Update(const glm::mat3x2 &Veticies, const glm::mat3x2 &TexCoords) const{
    std::vector<float> vertices{
            Veticies[0].x,Veticies[0].y,TexCoords[0].x / TexPtr->size.x,TexCoords[0].y / TexPtr->size.y,
            Veticies[1].x,Veticies[1].y,TexCoords[1].x / TexPtr->size.x,TexCoords[1].y / TexPtr->size.y,
            Veticies[2].x,Veticies[2].y,TexCoords[2].x / TexPtr->size.x,TexCoords[2].y / TexPtr->size.y
    };
    unsigned int indices[] = {
            0,1,2
    };

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}