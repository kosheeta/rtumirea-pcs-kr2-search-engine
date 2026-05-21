#ifndef STSE_MAIN_WINDOW_H
#define STSE_MAIN_WINDOW_H

#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
class QCloseEvent;

#include "../core/interfaces/IIndexer.h"
#include "../core/interfaces/ISearcher.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(IIndexer* indexer,
               ISearcher* searcher,
               QWidget* parent = nullptr);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onChooseFolderClicked();
    void onIndexClicked();
    void onSearchClicked();

private:
    void applyInitialState();
    void setIndexingState(bool indexing);
    void updateSearchAvailability();
    void clearResults();
    void appendResultLine(const QString& line);

    IIndexer* indexer_;
    ISearcher* searcher_;

    QLineEdit* pathEdit_ = nullptr;
    QPushButton* chooseFolderButton_ = nullptr;
    QPushButton* indexButton_ = nullptr;
    QLineEdit* queryEdit_ = nullptr;
    QPushButton* searchButton_ = nullptr;
    QScrollArea* resultsArea_ = nullptr;
    QWidget* resultsContainer_ = nullptr;
    QVBoxLayout* resultsLayout_ = nullptr;

    bool isIndexing_ = false;
    bool hasIndexedData_ = false;
};

#endif
