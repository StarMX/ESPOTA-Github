
#ifndef REPOSITORY
#define REPOSITORY "StarMX/ESPOTA-Github"
#endif
namespace espota
{

    typedef struct
    {
        char tag[50];
        char downloadUrl[200];
        // char *varsion = NULL;
    } version_t;

    class Github
    {

    private:
        const char *RealDownloadUrl(const char *filename);

    public:
        version_t version;
        bool Update();
        void Upgrade(const char *filename = "");
    };
    extern Github github;
} // namespace espota
