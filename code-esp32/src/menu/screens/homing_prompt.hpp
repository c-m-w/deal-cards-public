/// homing_prompt.hpp

namespace m
{
    class homing_prompt : public menu
    {
    public:

        std::atomic_bool _continue = false;

        void draw(LGFX_Sprite & sprite) override;
        void process_inputs() override;
    };

    inline homing_prompt _homing_prompt;
}
