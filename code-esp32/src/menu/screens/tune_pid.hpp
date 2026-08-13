/// tune_pid.hpp

namespace m
{
    class tune_pid : public menu
    {
    private:
        constexpr static int N_ENTRIES = 5;
        int _selected_idx = 0;

    public:

        void draw(LGFX_Sprite & sprite) override;
        void process_inputs() override;
    };

    inline tune_pid _tune_pid;
}
