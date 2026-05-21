#include "MainWindow.h"

#include <QCoreApplication>
#include <QCloseEvent>
#include <QDebug>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QStatusBar>

#include "../core/model/SearchResult.h"

MainWindow::MainWindow(IIndexer* indexer,
                       ISearcher* searcher,
                       QWidget* parent)
    : QMainWindow(parent),
      indexer_(indexer),
      searcher_(searcher) {
    setWindowTitle("STSE");
    resize(900, 600);

    auto* central = new QWidget(this);
    auto* rootLayout = new QVBoxLayout(central);

    auto* pathRow = new QHBoxLayout();
    pathEdit_ = new QLineEdit(central);
    pathEdit_->setReadOnly(true);
    chooseFolderButton_ = new QPushButton("Выбрать папку", central);
    indexButton_ = new QPushButton("Индексировать", central);
    pathRow->addWidget(pathEdit_);
    pathRow->addWidget(chooseFolderButton_);
    pathRow->addWidget(indexButton_);

    auto* searchRow = new QHBoxLayout();
    queryEdit_ = new QLineEdit(central);
    queryEdit_->setPlaceholderText("Введите запрос");
    searchButton_ = new QPushButton("Найти", central);
    searchRow->addWidget(queryEdit_);
    searchRow->addWidget(searchButton_);

    resultsArea_ = new QScrollArea(central);
    resultsArea_->setWidgetResizable(true);
    resultsContainer_ = new QWidget(resultsArea_);
    resultsLayout_ = new QVBoxLayout(resultsContainer_);
    resultsLayout_->setAlignment(Qt::AlignTop);
    resultsArea_->setWidget(resultsContainer_);

    rootLayout->addLayout(pathRow);
    rootLayout->addLayout(searchRow);
    rootLayout->addWidget(resultsArea_);

    setCentralWidget(central);
    statusBar()->showMessage("Готово");

    connect(chooseFolderButton_, &QPushButton::clicked, this, &MainWindow::onChooseFolderClicked);
    connect(indexButton_, &QPushButton::clicked, this, &MainWindow::onIndexClicked);
    connect(searchButton_, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(queryEdit_, &QLineEdit::returnPressed, this, &MainWindow::onSearchClicked);

    applyInitialState();
}

void MainWindow::applyInitialState() {
    indexButton_->setEnabled(false);
    updateSearchAvailability();
}

void MainWindow::setIndexingState(bool indexing) {
    isIndexing_ = indexing;
    chooseFolderButton_->setEnabled(!indexing);
    indexButton_->setEnabled(!indexing);
    updateSearchAvailability();
}

void MainWindow::updateSearchAvailability() {
    const bool canSearch = !isIndexing_ && hasIndexedData_;
    queryEdit_->setEnabled(canSearch);
    searchButton_->setEnabled(canSearch);
}

void MainWindow::onChooseFolderClicked() {
    const QString path = QFileDialog::getExistingDirectory(this, "Выберите папку", QString());
    if (path.isEmpty()) {
        return;
    }

    pathEdit_->setText(path);
    hasIndexedData_ = false;
    updateSearchAvailability();
    clearResults();
    indexButton_->setEnabled(true);
    statusBar()->showMessage("Папка выбрана");
}

void MainWindow::onIndexClicked() {
    const QString path = pathEdit_->text();
    if (path.isEmpty()) {
        statusBar()->showMessage("Сначала выберите папку");
        return;
    }

    setIndexingState(true);
    hasIndexedData_ = false;
    clearResults();
    statusBar()->showMessage("Индексирование...");

    try {
        indexer_->setStatusCallback([this](const IndexingStatus& status) {
            if (status.eventType == IndexingEventType::STARTED) {
                statusBar()->showMessage(
                    QString("Индексирование: %1 файлов")
                        .arg(status.totalFiles));
            } else if (status.eventType == IndexingEventType::PROGRESS) {
                statusBar()->showMessage(
                    QString("Индексирование: %1/%2, токенов: %3")
                        .arg(status.processedFiles)
                        .arg(status.totalFiles)
                        .arg(status.uniqueTokens));
            } else if (status.eventType == IndexingEventType::FINISHED) {
                hasIndexedData_ = status.processedFiles > 0;
                statusBar()->showMessage(
                    QString("Проиндексировано: %1 файлов, %2 токенов")
                        .arg(status.processedFiles)
                        .arg(status.uniqueTokens));
            } else if (status.eventType == IndexingEventType::WARNING) {
                statusBar()->showMessage(QString::fromStdString(status.message));
            }

            QCoreApplication::processEvents();
        });

        indexer_->setSourcePath(path.toStdString());
        indexer_->buildIndex();
    } catch (const std::exception& ex) {
        statusBar()->showMessage(QString("Ошибка индексации: ") + ex.what());
    }

    setIndexingState(false);
}

void MainWindow::clearResults() {
    while (resultsLayout_->count() > 0) {
        QLayoutItem* item = resultsLayout_->takeAt(0);
        if (item != nullptr) {
            delete item->widget();
            delete item;
        }
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (isIndexing_) {
        statusBar()->showMessage("Дождитесь завершения индексации");
        event->ignore();
        return;
    }

    event->accept();
}

void MainWindow::appendResultLine(const QString& line) {
    auto* label = new QLabel(line, resultsContainer_);
    label->setWordWrap(true);
    resultsLayout_->addWidget(label);
}

void MainWindow::onSearchClicked() {
    const QString query = queryEdit_->text().trimmed();
    qDebug() << "[onSearchClicked] query:" << query;

    if (query.isEmpty()) {
        statusBar()->showMessage("Введите запрос");
        clearResults();
        return;
    }

    clearResults();
    try {
        const auto results = searcher_->search(query.toLocal8Bit().toStdString());
        qDebug() << "[onSearchClicked] results count:" << results.size();

        if (results.empty()) {
            appendResultLine("Ничего не найдено");
            statusBar()->showMessage("Поиск завершен: 0 результатов");
            return;
        }

        for (const auto& result : results) {
            const QString type = result.matchType == MatchType::EXACT ? "EXACT" : "FUZZY";
            qDebug() << "[onSearchClicked]"
                     << "path=" << QString::fromLocal8Bit(result.path.c_str())
                     << "type=" << type
                     << "token=" << QString::fromLocal8Bit(result.matchedToken.c_str())
                     << "freq=" << result.frequency
                     << "distance=" << result.distance;

            appendResultLine(QString::fromLocal8Bit(result.path.c_str()) +
                             " | " + type +
                             " | token=" + QString::fromLocal8Bit(result.matchedToken.c_str()) +
                             " | freq=" + QString::number(result.frequency) +
                             " | distance=" + QString::number(result.distance));
        }
        statusBar()->showMessage(QString("Поиск завершен: %1 результатов").arg(results.size()));
    } catch (const std::exception& ex) {
        appendResultLine(QString("Ошибка поиска: ") + ex.what());
        statusBar()->showMessage("Ошибка поиска");
    }
}
