#include <QApplication>
#include "lonelymainwindow.h"
#include "romselectdialog.h"
#include "util/FileManip.h"
#include "util/IniFile.h"
#include "util/StringConversion.h"
#include <iostream>

using namespace Lonely;

class LonelyQApplication : public QApplication {
public:
    LonelyQApplication(int argc, char* argv[])
        : QApplication(argc, argv) { };

    bool notify(QObject* obj, QEvent* ev) {
        try {
        return QApplication::notify(obj, ev);
        }
        catch (OutOfRangeIndexException& e) {
            std::cerr << "OutOfRangeIndexException: " << std::endl;
            std::cerr << '\t' << "exception source: " << e.source() << std::endl;
            std::cerr << '\t' << "source file: " << e.nameOfSourceFile() << std::endl;
            std::cerr << '\t' << "source file line num: " << e.lineNum() << std::endl;
            std::cerr << '\t' << "invalid index: " << e.invalidIndex() << std::endl;
            std::terminate();
//            throw;
        }
        catch (TalesException& e) {
            std::cerr << "TalesException: " << std::endl;
            std::cerr << '\t' << "exception source: " << e.source() << std::endl;
            std::cerr << '\t' << "source file: " << e.nameOfSourceFile() << std::endl;
            std::cerr << '\t' << "source file line num: " << e.lineNum() << std::endl;
            std::terminate();
//            throw;
        }
    }
};

int main(int argc, char *argv[])
{
    LonelyQApplication a(argc, argv);

    IniFile configFile("lonely.ini");

    // Defaults
    if ((!(configFile.hasKey("Global", "OffsetFilePath")))) {
        configFile.setValue("Global", "OffsetFilePath",
                            "LaylaOffsets.txt");
    }

    if ((!(configFile.hasKey("Global", "SaveLastExportLocation")))) {
        configFile.setValue("Global", "SaveLastExportLocation",
                            "1");
    }

    // Verify that ROM path is set and ROM exists
    if ((!(configFile.hasKey("Global", "SourceRomPath")))
            || (!(FileManip::fileExists(
              (configFile.valueOfKey("Global", "SourceRomPath")))))) {
        configFile.setValue("Global", "SourceRomPath",
                            "");

        // If config file does not have source ROM path or that ROM does not
        // exist, do ROM select dialog
        RomSelectDialog romDialog(configFile);

        int result = romDialog.exec();

        // on cancel: can't continue without ROM, exit editor
        if (result == QDialog::Rejected) {
            return 0;
        }
    }

    LonelyMainWindow w(configFile);

//    try {
    w.show();
/*    }
    catch (TalesException& e) {
        std::cerr << "exception: " << std::endl;
        std::cerr << e.source() << std::endl;
        std::cerr << e.nameOfSourceFile() << std::endl;
        std::cerr << e.lineNum() << std::endl;
        throw;
    } */
    
    int result = a.exec();

    // Save config file after program finishes
    configFile.writeFile("lonely.ini");

    return result;
}
