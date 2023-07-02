#include<Transform.h>

void Transform::InsertIntoYaml(YAML::Emitter &Emitter) {
    Emitter << YAML::Key << "Translate";
    Position.InsertIntoYaml(Emitter);

    Emitter << YAML::Key << "Scale";
    Scale.InsertIntoYaml(Emitter);

    Emitter << YAML::Key << "Rotate";
    Rotation.InsertIntoYaml(Emitter);

}
