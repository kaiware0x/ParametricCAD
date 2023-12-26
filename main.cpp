
#include <QApplication>
#include <QHBoxLayout>
#include <QMainWindow>

#include "mainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    MainWindow::instance()->show();

    return app.exec();
}

////// load map
/// 済：EnsembleのDrawBoxを再現
/// 済：DrawBoxのRedoUndoを実装
/// 済：BoxのEdit、RedoUndo
/// 済：Parameter生成、RedoUndo
/// BoxへParameter適用、RedoUndo
/// Parameterを変更してBoxに反映、RedoUndo
///
///
/// MakeBox時点でParameterをセットできるようにする
/// MakeBoxとEditBoxでUI入力値由来なのかParameter由来なのかで分岐
/// ParameterはどのBoxから参照されているかを保持する
///     FunctionNodeListを保持
/// TDF_Referenceを用いて、各ArgumentLabel->ParameterLabelの参照を持たせる
///     Parameterが持つFunctionNodeListをIterate
///     FunctionNode->ArgumentsTagのChildIterate
///     ParameterへのRefを持っているArgを更新
///     紐づくShapeを再生成
///
///
