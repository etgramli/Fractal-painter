/* FractalPainter - This program paints several fractals; GUI using Qt (> 4.6)
 * Copyright (C) 2014  Etienne Gramlich
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <cstdio>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Load localization
    QTranslator editTranslator;
    QString filename;
    filename = QString("Fraktal_%1").arg(QLocale::system().name());
    editTranslator.load(filename, qApp->applicationDirPath() );
    a.installTranslator(&editTranslator);

    // Redirect stdout and stderr to corresponding (log-)files
    FILE *out = freopen("log_out.txt", "w", stdout);
    FILE *err = freopen("log_error.txt", "w", stderr);

    MainWindow w;
    w.show();
    int retVal = a.exec();

    fclose(stdout);
    fclose(stderr);
    fclose(out);
    fclose(err);

    return retVal;
}
