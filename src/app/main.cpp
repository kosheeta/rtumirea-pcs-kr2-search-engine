#include <QApplication>

#include <string>

#include "../core/factory/IModuleFactory.h"
#include "../core/factory/ModuleFactory.h"
#include "../gui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    bool useMocks = false;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--use-mocks") {
            useMocks = true;
        }
    }

    IModuleFactory* factory = new ModuleFactory();

    auto fileReader = factory->createFileReader();
    auto tokenizer = factory->createTokenizer();
    auto invertedIndex = factory->createInvertedIndex();
    auto indexer = factory->createIndexer(fileReader, tokenizer, invertedIndex);
    auto searcher = factory->createSearcher(tokenizer, invertedIndex);

    MainWindow window(indexer, searcher);
    window.show();

    int exitCode = app.exec();

    delete searcher;
    delete indexer;
    delete invertedIndex;
    delete tokenizer;
    delete fileReader;
    delete factory;
    factory = nullptr;

    return exitCode;
}
