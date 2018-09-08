template <typename T>
struct V2{

};

using V2<float> V2f;
using V2<uint32_t> V2ui;

struct UnitHasher{

};

struct Sprite {

};

struct Renderer {

};

struct UnitBase {
    V2f m_position;
    uint32_t m_id;
    V2f m_velocity;
    MovementAction m_movement_action;
    Sprite m_sprite;
};

struct ObstacleBase {

};

struct ObstacleHash {

};

struct GameData {
    Hashmap<PropBase, PropHash, 4096, 2048> m_game_map;
};

void update_position(UnitBase& unit, float delta_time) {

}

void update_unit_AI(UnitBase& unit) {

}

V2 calculate_camera_pos(UnitBase& unit, Camera& camera) {

}

void render_unit(UnitBase& unit) {
    renderer.push_sprite(unit.m_sprite, calculate_camera_pos(unit, camera));
}

void game_loop(float delta_time) {
    iter = m_game_map.bucket_iterator();
    while (iter.next()) {
        if (!iter.occupied) {
            continue;
        }
        update_position(*iter, delta_time);
        update_unit_AI(*iter);
        render_unit(*iter);
    }
    renderer.render_all();
}

